
#include "ft_ping.h"

static int recv_ping_msg(struct msghdr *msg, int sequence) {
    int received_size = recvmsg(ping.sockfd, msg, 0);
    if (received_size < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)   // Timeout occurred
        {
            if (ping.args.v_flag && ping.args.q_flag == false) {
                printf("Request timeout for icmp_seq %d\n", sequence);
            }
            return -1;
        } else {
            if (ping.args.v_flag && ping.args.q_flag == false) {
                fprintf(stderr, "%s: recvmsg: %s\n", PROGRAM_NAME, strerror(errno));
            }
            return -1;
        }
    }
    return received_size;
}

static void process_received_ping(int received_size, struct msghdr *msg, int sequence) {
    struct icmp icmp;
    struct ip *ip_header = (struct ip *) msg->msg_iov->iov_base;
    int ip_header_length = ip_header->ip_hl << 2;

    ft_bzero(&icmp, sizeof(struct icmp));
    ft_memcpy(&icmp, (char *) ip_header + ip_header_length, sizeof(struct icmp));
    if (icmp.icmp_type == ICMP_ECHOREPLY && icmp.icmp_id == (getpid() & 0xffff) &&
        icmp.icmp_seq == sequence) {
        handle_ICMP_echo_package(received_size, icmp, msg->msg_name, ip_header);
    } else if (icmp.icmp_type == ICMP_TIMXCEED) {
        handle_ttl_package(received_size, icmp, msg->msg_name, ip_header, sequence);

    } else if (ping.args.v_flag) {
        printf("Received packet for sequence %d is not an ICMP echo reply: \n", sequence);
        printf("IP Hdr Dump:\n");
        ft_hexdump(ip_header, sizeof(struct ip));
        printf("ICMP: type %d, code %d, size %ld, id 0x%x, seq 0x%x\n", icmp.icmp_type,
               icmp.icmp_code, sizeof(icmp), icmp.icmp_id, icmp.icmp_seq);
    }
}

/*
 * For some reason setting in receive_ping directly was not working, I guess it is because of static
 * char or something like that.
 */
void set_msg_header(struct msghdr *msghdr, struct iovec *iov, char buffer[IP_MAXPACKET],
                    struct sockaddr_in sockaddr) {
    ft_bzero(msghdr, sizeof(struct msghdr));
    msghdr->msg_name = &sockaddr;
    msghdr->msg_namelen = sizeof(struct sockaddr_in);
    msghdr->msg_iov = iov;
    msghdr->msg_iovlen = 1;
    msghdr->msg_control = buffer;
    msghdr->msg_controllen = IP_MAXPACKET;
    msghdr->msg_flags = 0;
}

void receive_ping(int sequence) {
    char buffer[IP_MAXPACKET];
    struct iovec iov;
    struct msghdr msg;
    int received_size;

    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);

    set_msg_header(&msg, &iov, buffer, ping.server_addr);

    if ((received_size = recv_ping_msg(&msg, sequence)) >= 0) {
        process_received_ping(received_size, &msg, sequence);
    }
}