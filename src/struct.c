#include "ft_ping.h"

/*
 * This function is used to initialize the packets_stats structure
 */
void set_packets_stats() {
    ping.packets_stats.transmitted = 0;
    ping.packets_stats.received = 0;
    ping.packets_stats.min_rtt = DBL_MAX;
    ping.packets_stats.max_rtt = -DBL_MAX;
    ping.packets_stats.sum_rtt = 0;
    ping.packets_stats.sum_squared_rtt = 0;
}

void set_args_structure() {
    ping.args.a_flag = false;
    ping.args.D_flag = false;
    ping.args.h_flag = false;
    ping.args.i_flag = false;
    ping.args.n_flag = false;
    ping.args.v_flag = false;
    ping.args.ttl_value = DEFAULT_TTL;
    ping.args.w_flag = false;
    ping.args.deadline = 0;
    ping.args.interval = 1;
    ping.args.host = NULL;
    ping.args.num_packets = -1;
}