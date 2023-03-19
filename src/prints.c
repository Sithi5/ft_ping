#include "ft_ping.h"

void print_ping_address_infos(struct sockaddr_in *server_addr) {
    char ip_address[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(server_addr->sin_addr), ip_address, INET_ADDRSTRLEN);

    printf("PING %s (%s): %lu data bytes\n", ping.args.host, ip_address, sizeof(struct icmp));
}

void print_statistics() {
    double avg_rtt = ping.packets_stats.sum_rtt / ping.packets_stats.received;
    double variance =
        (ping.packets_stats.sum_squared_rtt / ping.packets_stats.received) - (avg_rtt * avg_rtt);
    double stddev_rtt = ft_sqrt(variance);

    printf("\n--- %s ping statistics ---\n", ping.args.host);

    printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n",
           ping.packets_stats.transmitted, ping.packets_stats.received,
           (1.0 - ping.packets_stats.received / ping.packets_stats.transmitted) * 100.0);
    if (ping.packets_stats.received > 0) {
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping.packets_stats.min_rtt, avg_rtt, ping.packets_stats.max_rtt, stddev_rtt);
    }
}