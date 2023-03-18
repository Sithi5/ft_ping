
#include "ft_ping.h"

static struct icmp create_icmp_header(uint16_t sequence)
{
    struct icmp icmp;

    ft_bzero(&icmp, sizeof(icmp));

    icmp.icmp_type = ICMP_ECHO;                                             // ICMP echo request
    icmp.icmp_code = 0;                                                     // Always 0
    icmp.icmp_id = getpid() & 0xffff;                                       // ID of the process, we are using our own PID here (16 bits) to keep it simple
    icmp.icmp_seq = sequence;                                               // Sequence number
    gettimeofday((struct timeval *)icmp.icmp_data, NULL);                   // Store timestamp in icmp payload
    icmp.icmp_cksum = ft_icmp_checksum((char *)&icmp, sizeof(struct icmp)); // Calculate the checksum of the ICMP header
    return icmp;
}

int send_ping(int sockfd, t_args *args, struct sockaddr_in server_addr, uint16_t sequence, t_packets_stats *packets_stats)
{
    int packet_size = sizeof(struct icmp);
    t_packet packet;

    ft_bzero(&packet, sizeof(t_packet));

    packet.icmp = create_icmp_header(sequence);
    // packet.ip = create_ip_header(server_addr);
    if (DEBUG)
        printf("packet_size: %d", packet_size);
    int ret = sendto(sockfd, &packet.icmp, packet_size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        fprintf(stderr, "%s: sendto: %s\n", PROGRAM_NAME, strerror(errno));
    }
    else
    {
        packets_stats->transmitted++;
    }
    return 0;
}