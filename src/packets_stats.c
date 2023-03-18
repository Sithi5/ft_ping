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
