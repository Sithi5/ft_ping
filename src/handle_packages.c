
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

void handle_ICMP_echo_package(int received_size, struct icmp icmp, struct sockaddr *server_addr,
                              struct ip *ip_header) {
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

void handle_ttl_package(int received_size, struct sockaddr *server_addr) {
    char ip_address[INET_ADDRSTRLEN];
    char *dns_name;
    struct timeval end_time;

    gettimeofday(&end_time, NULL);
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
                printf("%d bytes from %s: ", received_size, dns_name);
            } else {
                printf("%d bytes from %s: ", received_size, ip_address);
            }
            printf("Time to live exceeded\n");
        }
    }
}