
#include "ft_ping.h"

void receive_ping(int sockfd, t_args *args, struct sockaddr *addr, uint16_t sequence)
{
    struct icmp icmp;
    t_packet_stats packet_stats;
    struct msghdr msg;
    struct iovec iov;
    char buffer[IP_MAXPACKET];

    msg.msg_name = addr,
    msg.msg_namelen = sizeof(*addr),
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (DEBUG)
        printf("Waiting for received packets...\n");
    packet_stats.received_size = recvmsg(sockfd, &(msg), 0);
    if (packet_stats.received_size < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // Timeout occurred
        {
            fprintf(stderr, "%s: recvmsg: Timeout\n", PROGRAM_NAME);
            return;
        }
        else
        {
            fprintf(stderr, "%s: recvmsg: %s\n", PROGRAM_NAME, strerror(errno));
            exit_clean(sockfd, ERROR_RECVFROM);
        }
    }
    if (DEBUG)
        printf("received packets...\n");

    struct ip *ip_header = (struct ip *)msg.msg_iov->iov_base;
    int ip_header_length = ip_header->ip_hl << 2;
    ft_memcpy(&icmp, (char *)ip_header + ip_header_length, sizeof(struct icmp));

    // Check if the received packet is an ICMP echo reply packet and if it is the one we sent (by checking the ID and sequence number)
    if (icmp.icmp_type == ICMP_ECHOREPLY && icmp.icmp_id == (getpid() & 0xffff) && icmp.icmp_seq == sequence)
    {
        // Calculate the round-trip time (RTT) of the packet
        struct timeval *sent_time = (struct timeval *)icmp.icmp_data;
        gettimeofday(&(packet_stats.end_time), NULL);
        packet_stats.rtt = ((double)(packet_stats.end_time.tv_sec - sent_time->tv_sec) * 1000.0) + ((double)(packet_stats.end_time.tv_usec - sent_time->tv_usec) / 1000.0);

        packet_stats.ttl = ip_header->ip_ttl;

        // Print the statistics of the received packet
        printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", packet_stats.received_size, inet_ntoa(((struct sockaddr_in *)addr)->sin_addr), icmp.icmp_seq, packet_stats.ttl, packet_stats.rtt);
    }
    else
    {
        if (DEBUG)
        {
            printf("Received packet: %s\n", buffer);
            printf("Received packet size: %d\n", packet_stats.received_size);
            printf("Received packet type: %d\n", icmp.icmp_type);
        }
    }
}