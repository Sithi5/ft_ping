
#include "ft_ping.h"

static void update_packets_stats(double rtt) {
    ping.packets_stats.received++;

    // Update min_rtt
    if (ping.packets_stats.min_rtt > rtt) {
        ping.packets_stats.min_rtt = rtt;
    }

    // Update max_rtt
    if (ping.packets_stats.max_rtt < rtt) {
        ping.packets_stats.max_rtt = rtt;
    }

    ping.packets_stats.sum_rtt += rtt;
    ping.packets_stats.sum_squared_rtt += rtt * rtt;
}

static void handle_ICMP_echo_package(int received_size, struct icmp icmp,
                                     struct sockaddr *server_addr, struct ip *ip_header) {
    struct timeval sent_time = *(struct timeval *) icmp.icmp_data;
    struct timeval end_time;
    char ip_address[INET_ADDRSTRLEN];
    char *dns_name;
    double rtt;

    gettimeofday(&end_time, NULL);
    rtt = calculate_package_rtt(&sent_time, &end_time);
    update_packets_stats(rtt);

    inet_ntop(AF_INET, &(((struct sockaddr_in *) server_addr)->sin_addr), ip_address,
              INET_ADDRSTRLEN);

    dns_name = ft_reverse_dns_lookup(server_addr, NI_MAXHOST);

    if (ping.args.q_flag == false) {
        if (ping.args.a_flag)
            printf("\a");
        else {
            if (ping.args.D_flag)
                printf("[%ld.%06ld] ", (long) end_time.tv_sec, (long) end_time.tv_usec);
            if (ping.args.n_flag == false) {
                printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", received_size,
                       dns_name, icmp.icmp_seq, ip_header->ip_ttl, rtt);
            } else {
                printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", received_size,
                       ip_address, icmp.icmp_seq, ip_header->ip_ttl, rtt);
            }
        }
    }
}

static void handle_ttl_package(struct sockaddr *server_addr, struct icmp icmp) {
    char ip_address[INET_ADDRSTRLEN];
    char *dns_name;

    inet_ntop(AF_INET, &(((struct sockaddr_in *) server_addr)->sin_addr), ip_address,
              INET_ADDRSTRLEN);

    dns_name = ft_reverse_dns_lookup(server_addr, NI_MAXHOST);

    if (ping.args.v_flag && ping.args.q_flag == false) {
        if (ping.args.n_flag == false) {
            printf("From %s: icmp_seq=%d Time to live exceeded\n", dns_name, icmp.icmp_seq);

        } else {
            printf("From %s: icmp_seq=%d Time to live exceeded\n", ip_address, icmp.icmp_seq);
        }
    }
}

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
    // Check if the received packet is an ICMP echo reply packet and if it is the one we sent
    // (by checking the ID and sequence number)
    if (icmp.icmp_type == ICMP_ECHOREPLY && icmp.icmp_id == (getpid() & 0xffff) &&
        icmp.icmp_seq == sequence) {
        handle_ICMP_echo_package(received_size, icmp, msg->msg_name, ip_header);
    } else if (icmp.icmp_type == ICMP_TIMXCEED) {
        handle_ttl_package(msg->msg_name, icmp);
    } else if (ping.args.v_flag) {
        // Print the packet if the verbose option is enabled (-v) and the packet is not an ICMP echo
        // reply packet
        printf("Received packet for sequence %d is not an ICMP echo reply: \n", sequence);
        printf("Received packet: %s\n", (char *) msg->msg_iov->iov_base);
        printf("Received packet size: %d\n", received_size);
        printf("Received packet type: %d\n", icmp.icmp_type);
    }
}

void receive_ping(int sequence) {
    char buffer[IP_MAXPACKET];
    struct iovec iov;
    struct msghdr msg;
    int received_size;

    ft_bzero(&msg, sizeof(msg));
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_name = (struct sockaddr *) &ping.server_addr;
    msg.msg_namelen = sizeof(*(struct sockaddr *) &ping.server_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if ((received_size = recv_ping_msg(&msg, sequence)) >= 0) {
        process_received_ping(received_size, &msg, sequence);
    }
}