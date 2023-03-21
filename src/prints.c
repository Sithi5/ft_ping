#include "ft_ping.h"

void print_ping_address_infos() {
    char ip_address[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(ping.server_addr.sin_addr), ip_address, INET_ADDRSTRLEN);

    if (ping.args.v_flag) {
        printf("PING %s (%s): %lu data bytes, id 0x%x = %d\n", ping.args.host, ip_address,
               sizeof(struct icmp), getpid() & 0xffff, getpid() & 0xffff);
    } else {
        printf("PING %s (%s): %lu data bytes\n", ping.args.host, ip_address, sizeof(struct icmp));
    }
}

void print_statistics() {
    double avg_rtt = ping.packets_stats.sum_rtt / ping.packets_stats.received;
    double variance =
        (ping.packets_stats.sum_squared_rtt / ping.packets_stats.received) - (avg_rtt * avg_rtt);
    double stddev_rtt = ft_sqrt(variance);

    printf("--- %s ping statistics ---\n", ping.args.host);

    printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n",
           ping.packets_stats.transmitted, ping.packets_stats.received,
           (1.0 - ping.packets_stats.received / ping.packets_stats.transmitted) * 100.0);
    if (ping.packets_stats.received > 0) {
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping.packets_stats.min_rtt, avg_rtt, ping.packets_stats.max_rtt, stddev_rtt);
    }
}

void display_received_package_infos(struct ip *ip_header, int sequence) {
    char src_ip_address[INET_ADDRSTRLEN];
    char dst_ip_address[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ip_header->ip_src, src_ip_address, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ping.server_addr.sin_addr.s_addr, dst_ip_address, INET_ADDRSTRLEN);
    printf("IP Hdr Dump:\n ");
    ft_hexdump(ip_header, sizeof(struct ip));
    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    printf("%2d ", ip_header->ip_v);                             // Vr
    printf("%2d ", ip_header->ip_hl);                            // HL
    printf("%02x ", ip_header->ip_tos);                          // TOS
    printf("%04x ", ft_ntohs(ip_header->ip_len));                // Len
    printf("%04x  ", ft_ntohs(ip_header->ip_id));                // ID
    printf("%2d ", (ft_ntohs(ip_header->ip_off) >> 13) & 0x7);   // Flg
    printf("%04x ", ft_ntohs(ip_header->ip_off) & 0x1FFF);       // off
    printf("%3d ", ip_header->ip_ttl);                           // TTL
    printf("%02d ", ip_header->ip_p);                            // Pro
    printf("%04x ", ft_ntohs(ip_header->ip_sum));                // cks
    printf("%s  ", src_ip_address);                              // Src
    printf("%s ", dst_ip_address);                               // Dst
    printf("\n");
    printf("ICMP: type %d, code %d, size %ld, id 0x%x, seq 0x%x\n", ICMP_ECHO, ICMP_ECHOREPLY,
           sizeof(struct icmp), getpid() & 0xffff, sequence);
}