
#include "ft_ping.h"

void update_packets_stats(t_packets_stats *packets_stats, int received_size, double rtt)
{
    packets_stats->received++;

    // Update min_rtt
    if (packets_stats->min_rtt > rtt)
    {
        packets_stats->min_rtt = rtt;
    }

    // Update max_rtt
    if (packets_stats->max_rtt < rtt)
    {
        packets_stats->max_rtt = rtt;
    }

    packets_stats->sum_rtt += rtt;
    packets_stats->sum_squared_rtt += rtt * rtt;
}

void handle_ICMP_echo_package(int received_size, struct icmp icmp, struct sockaddr *server_addr, struct ip *ip_header, t_packets_stats *packets_stats)
{
    struct timeval *sent_time = (struct timeval *)icmp.icmp_data;
    struct timeval end_time;
    unsigned int ttl;
    char ip_address[INET_ADDRSTRLEN];
    double rtt;

    gettimeofday(&end_time, NULL);
    // Calculate the round-trip time (RTT) of the packet
    rtt = ((double)(end_time.tv_sec - sent_time->tv_sec) * 1000.0) + ((double)(end_time.tv_usec - sent_time->tv_usec) / 1000.0);
    ttl = ip_header->ip_ttl;
    update_packets_stats(packets_stats, received_size, rtt);
    inet_ntop(AF_INET, &(((struct sockaddr_in *)server_addr)->sin_addr), ip_address, INET_ADDRSTRLEN);
    printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", received_size, ip_address, icmp.icmp_seq, ttl, rtt);
}

void receive_ping(int sockfd, t_args *args, t_packets_stats *packets_stats, struct sockaddr *addr, uint16_t sequence)
{
    struct icmp icmp;
    struct msghdr msg;
    struct iovec iov;
    char buffer[IP_MAXPACKET];
    int received_size;

    msg.msg_name = addr,
    msg.msg_namelen = sizeof(*addr),
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (DEBUG)
        printf("Waiting for received packets...\n");
    received_size = recvmsg(sockfd, &(msg), 0);
    if (received_size < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) // Timeout occurred
        {
            printf("Request timeout for icmp_seq %hu\n", sequence);
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
        handle_ICMP_echo_package(received_size, icmp, addr, ip_header, packets_stats);
    }
    else
    {
        if (DEBUG)
        {
            printf("Received packet: %s\n", buffer);
            printf("Received packet size: %d\n", received_size);
            printf("Received packet type: %d\n", icmp.icmp_type);
        }
    }
}