#include "ft_ping.h"

g_ping ping;

void int_handler(int signo) {
    if (ping.packets_stats.transmitted > 0) {
        print_statistics();
    }
    exit_clean(ping.sockfd, ERROR_SIGINT);
    (void) signo;
}

void create_socket() {
    struct timeval timeout;

    // Create a raw socket with ICMP protocol, AF_INET is the address family for
    // IPv4
    ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping.sockfd < 0) {
        fprintf(stderr, "%s: socket: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPEN);
    }

    if (setsockopt(ping.sockfd, IPPROTO_IP, IP_TTL, &ping.args.ttl_value,
                   sizeof(ping.args.ttl_value)) < 0) {
        fprintf(stderr, "%s: setsockopt: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPTION);
    }

    timeout.tv_sec = 0;         // 0 second
    timeout.tv_usec = 100000;   // 100000 microseconds

    if (setsockopt(ping.sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        fprintf(stderr, "%s: setsockopt: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPTION);
    }
}

void resolve_server_addr() {
    int status;
    struct sockaddr_in *server_addrs;

    ping.server_addr.sin_family = AF_INET;   // IPv4
    ping.server_addr.sin_port = 0;           // Use default port
    status = inet_pton(AF_INET, ping.args.host, &ping.server_addr.sin_addr);
    if (status == 0) {   // Input is not an IPv4 address, try resolving as a domain name
        if (!(server_addrs = ft_gethostbyname(ping.args.host, 1))) {
            fprintf(stderr, "%s: cannot resolve %s: Unknow host\n", PROGRAM_NAME, ping.args.host);
            exit_clean(ping.sockfd, ERROR_RESOLVING_HOST);
        }
        ping.server_addr.sin_addr = server_addrs[0].sin_addr;   // Use first address
    } else if (status < 0) {
        fprintf(stderr, "%s: inet_pton: %s\n", PROGRAM_NAME, strerror(errno));
        exit_clean(ping.sockfd, ERROR_INET_PTON);
    }
}

int main(int argc, char *argv[]) {

    set_args_structure();
    set_packets_stats();
    parse_args(argc, argv);
    create_socket();
    signal(SIGINT, int_handler);
    resolve_server_addr();
    print_ping_address_infos();
    alarm(ping.args.deadline);
    for (int sequence = 0; 1; sequence++) {
        send_ping(sequence);
        receive_ping(sequence);
        if (ping.args.num_packets > 0 &&
            (ping.args.num_packets == ping.packets_stats.transmitted)) {
            break;
        }
        usleep((int) (ping.args.interval * 1000000));
    }
    print_statistics();
    close(ping.sockfd);
    return 0;
}