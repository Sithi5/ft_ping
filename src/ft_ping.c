#include "ft_ping.h"

int sockfd = -1;

void int_handler(int signo)
{
    exit_clean(sockfd, ERROR_SIGINT);
}

int send_ping(t_args *args, struct sockaddr_in server_addr, uint16_t sequence)
{
    t_icmp_header icmp;
    int packet_size = sizeof(t_icmp_header);
    ft_bzero(&icmp, packet_size);

    icmp.type = ICMP_ECHO; // ICMP echo request
    icmp.code = 0;
    icmp.id = getpid() & 0xffff;
    icmp.sequence = sequence;
    gettimeofday(&icmp.timestamp, NULL);
    icmp.checksum = ft_icmp_checksum((char *)&icmp, packet_size);
    int ret = sendto(sockfd, &icmp, packet_size, 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        fprintf(stderr, "%s: sendto: %s\n", PROGRAM_NAME, strerror(errno));
    }
    return 0;
}

void receive_ping(t_args *args, struct sockaddr *addr, uint16_t sequence)
{
    t_icmp_header icmp;
    t_packet_stats packet_stats;
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct iovec iov;
    char buffer[IP_MAXPACKET];

    msg.msg_name = addr,
    msg.msg_namelen = sizeof(*addr),
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    DEBUG ? printf("Waiting for received packets...\n") : 0;
    packet_stats.received_size = recvmsg(sockfd, &(msg), 0);
    if (packet_stats.received_size < 0)
    {
        fprintf(stderr, "%s: recvmsg: %s\n", PROGRAM_NAME, strerror(errno));
        exit_clean(sockfd, ERROR_RECVFROM);
    }
    DEBUG ? printf("received packets...\n") : 0;

    ft_memcpy(&icmp, msg.msg_iov->iov_base, sizeof(t_icmp_header));

    // Check if the received packet is an ICMP echo reply packet and if it is the one we sent (by checking the ID and sequence number)
    if (icmp.type == ICMP_ECHOREPLY && icmp.id == (getpid() & 0xffff) && icmp.sequence == sequence)
    {
        // Calculate the round-trip time (RTT) of the packet
        gettimeofday(&(packet_stats.end_time), NULL);
        packet_stats.rtt = ((double)(packet_stats.end_time.tv_sec - icmp.timestamp.tv_sec) * 1000.0) + ((double)(packet_stats.end_time.tv_usec - icmp.timestamp.tv_usec) / 1000.0);

        // Extract the TTL value from the control msg header
        for (cmsg = CMSG_FIRSTHDR(&(msg)); cmsg != NULL; cmsg = CMSG_NXTHDR(&(msg), cmsg))
        {
            if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
            {
                packet_stats.ttl = *(uint8_t *)CMSG_DATA(cmsg);
                break;
            }
        }

        // Print the statistics of the received packet
        printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", packet_stats.received_size, inet_ntoa(((struct sockaddr_in *)addr)->sin_addr), icmp.sequence, packet_stats.ttl, packet_stats.rtt);
    }
    else
    {
        DEBUG ? printf("Received packet: %s\n", buffer) : 0;
        DEBUG ? printf("Received packet size: %d\n", packet_stats.received_size) : 0;
        DEBUG ? printf("Received packet type: %d\n", icmp.type) : 0;
    }
}

void create_socket()
{
    // Create a raw socket with ICMP protocol, AF_INET is the address family for IPv4
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "%s: socket: %s\n", PROGRAM_NAME, strerror(errno));
        exit(ERROR_SOCKET_OPEN);
    }
    int optval = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_RECVERR, &optval, sizeof(optval)) < 0)
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
            exit_clean(sockfd, ERROR_RESOLVING_HOST);
        }
        ipv4_addr = *((struct in_addr *)(he->h_addr_list[0]));
    }
    else if (status < 0)
    { // Conversion error
        fprintf(stderr, "%s: inet_pton: %s\n", PROGRAM_NAME, strerror(errno));
        exit_clean(sockfd, ERROR_INET_PTON);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 0; // Use default port
    server_addr.sin_addr = ipv4_addr;

    printf("PING %s (%s): %lu data bytes\n",
           inet_ntoa(server_addr.sin_addr),
           inet_ntoa(server_addr.sin_addr),
           sizeof(t_icmp_header));
    for (int i = 0; args.num_packets < 0 || i < args.num_packets; i++)
    {
        DEBUG ? printf("\nSending ping to %s...\n", inet_ntoa(server_addr.sin_addr)) : 0;
        send_ping(&args, server_addr, i);
        receive_ping(&args, (struct sockaddr *)&server_addr, i);
        sleep(1);
    }

    close(sockfd);
    return 0;
}