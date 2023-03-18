#include "ft_ping.h"

void set_packets_stats(t_packets_stats *packets_stats)
{
    packets_stats->transmitted = 0;
    packets_stats->received = 0;
    packets_stats->min_rtt = DBL_MAX;
    packets_stats->max_rtt = -DBL_MAX;
    packets_stats->sum_rtt = 0;
    packets_stats->sum_squared_rtt = 0;
}

void print_statistics(t_packets_stats *packets_stats, const char *host)
{
    double avg_rtt = packets_stats->sum_rtt / packets_stats->received;
    double variance = (packets_stats->sum_squared_rtt / packets_stats->received) - (avg_rtt * avg_rtt);
    double stddev_rtt = ft_sqrt(variance);

    printf("\n--- %s ping statistics ---\n", host);

    printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n",
           packets_stats->transmitted, packets_stats->received,
           (1.0 - packets_stats->received / packets_stats->transmitted) * 100.0);
    if (packets_stats->received > 0)
    {
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               packets_stats->min_rtt, avg_rtt, packets_stats->max_rtt, stddev_rtt);
    }
}