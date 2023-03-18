#include "ft_ping.h"

g_ping ping;

void int_handler(int signo)
{
    exit_clean(ping.sockfd, ERROR_SIGINT);
}

void create_socket()
{
    struct timeval timeout;

    // Create a raw socket with ICMP protocol, AF_INET is the address family for IPv4
    ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping.sockfd < 0)
    {
        fprintf(stderr, "%s: socket: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPEN);
    }

    timeout.tv_sec = 0;       // 0 second
    timeout.tv_usec = 100000; // 100000 microseconds

    if (setsockopt(ping.sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        fprintf(stderr, "%s: setsockopt: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPTION);
    }
}

int main(int argc, char *argv[])
{
    int status;
    t_args args;
    struct hostent *he;
    struct in_addr ipv4_addr;
    struct sockaddr_in server_addr;
    t_packets_stats packets_stats;

    set_packets_stats(&packets_stats);

    create_socket();
    // The SIGINT signal is sent to a program when the user presses Ctrl+C, closing the program
    signal(SIGINT, int_handler);
    set_args_structure(&args);
    parse_args(argc, argv, &args);

    status = inet_pton(AF_INET, args.host, &ipv4_addr);
    if (status == 0)
    { // Input is not an IPv4 address, try resolving as a domain name
        he = gethostbyname(args.host);
        if (he == NULL)
        {
            fprintf(stderr, "%s: cannot resolve %s: Unknow host\n", PROGRAM_NAME, args.host);
            exit_clean(ping.sockfd, ERROR_RESOLVING_HOST);
        }
        ipv4_addr = *((struct in_addr *)(he->h_addr_list[0]));
    }
    else if (status < 0)
    { // Conversion error
        fprintf(stderr, "%s: inet_pton: %s\n", PROGRAM_NAME, strerror(errno));
        exit_clean(ping.sockfd, ERROR_INET_PTON);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 0; // Use default port
    server_addr.sin_addr = ipv4_addr;

    print_ping_address_infos(&args, &server_addr);
    for (int sequence = 0; args.num_packets < 0 || sequence < args.num_packets; sequence++)
    {
        send_ping(server_addr, sequence);
        receive_ping(ping.sockfd, &args, &packets_stats, (struct sockaddr *)&server_addr, sequence);
        usleep((int)(args.interval * 1000000));
    }
    print_statistics(&packets_stats, args.host);

    close(ping.sockfd);
    return 0;
}