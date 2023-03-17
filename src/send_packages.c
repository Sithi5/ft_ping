
#include "ft_ping.h"

static struct icmp create_icmp_header(uint16_t sequence)
{
    struct icmp icmp;

    ft_bzero(&icmp, sizeof(icmp));

    icmp.icmp_type = ICMP_ECHO; // ICMP echo request
    icmp.icmp_code = 0;
    icmp.icmp_id = getpid() & 0xffff;
    icmp.icmp_seq = sequence;
    icmp.icmp_cksum = ft_icmp_checksum((char *)&icmp, sizeof(struct icmp));
    return icmp;
}

int send_ping(int sockfd, t_args *args, struct sockaddr_in server_addr, uint16_t sequence)
{
    int packet_size = sizeof(struct icmp);
    t_packet packet;

    ft_bzero(&packet, sizeof(t_packet));

    packet.icmp = create_icmp_header(sequence);
    // packet.ip = create_ip_header(server_addr);
    DEBUG ? printf("packet_size: %d", packet_size) : 0;
    int ret = sendto(sockfd, &packet.icmp, packet_size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        fprintf(stderr, "%s: sendto: %s\n", PROGRAM_NAME, strerror(errno));
    }
    return 0;
}