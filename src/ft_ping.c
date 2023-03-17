#include "ft_ping.h"

int sockfd = -1;

void int_handler(int signo)
{
    exit_clean(sockfd, ERROR_SIGINT);
}

void send_ping(t_args *args, struct sockaddr *dest_addr, uint16_t sequence)
{
    t_icmp_header icmp;
    int packet_size = sizeof(icmp);

    ft_bzero(&icmp, packet_size);

    icmp.type = ICMP_ECHO; // ICMP echo request
    icmp.code = 0;
    icmp.checksum = 0;
    icmp.id = getpid() & 0xffff;
    icmp.sequence = sequence;
    gettimeofday(&icmp.timestamp, NULL);

    char dest_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(((struct sockaddr_in *)dest_addr)->sin_addr), dest_ip, INET_ADDRSTRLEN);
    printf("Sending ping to %s\n", dest_ip);

    icmp.checksum = ft_icmp_checksum((char *)&icmp, packet_size);

    if (sendto(sockfd, &icmp, packet_size, MSG_DONTROUTE, dest_addr, sizeof(*dest_addr)) < 0)
    {
        if (errno == EHOSTUNREACH || errno == ENETUNREACH)
        {
            fprintf(stderr, "%s: sendto: no route to host\n", PROGRAM_NAME);
        }
        else
        {
            fprintf(stderr, "%s: sendto: %s\n", PROGRAM_NAME, strerror(errno));
            exit_clean(sockfd, ERROR_SENDTO);
        }
    }
}

void receive_ping(t_args *args, t_packet_stats *packet_stats, t_packet *packet, struct sockaddr *addr, uint16_t sequence)
{
    t_icmp_header icmp;

    packet->msg.msg_name = addr,
    packet->msg.msg_namelen = sizeof(*addr),

    packet_stats->received_size = recvmsg(sockfd, &(packet->msg), 0);
    if (packet_stats->received_size < 0)
    {
        fprintf(stderr, "%s: recvmsg: %s\n", PROGRAM_NAME, strerror(errno));
        exit_clean(sockfd, ERROR_RECVFROM);
    }

    ft_memcpy(&icmp, packet->msg.msg_iov->iov_base, packet->packet_size);

    // Check if the received packet is an ICMP echo reply packet
    if (icmp.type == ICMP_ECHOREPLY && icmp.id == (getpid() & 0xffff) && icmp.sequence == sequence)
    {
        // Calculate the round-trip time (RTT) of the packet
        gettimeofday(&(packet_stats->end_time), NULL);
        packet_stats->rtt = ((double)(packet_stats->end_time.tv_sec - icmp.timestamp.tv_sec) * 1000.0) + ((double)(packet_stats->end_time.tv_usec - icmp.timestamp.tv_usec) / 1000.0);

        // Extract the TTL value from the control msg header
        for (packet->cmsg = CMSG_FIRSTHDR(&(packet->msg)); packet->cmsg != NULL; packet->cmsg = CMSG_NXTHDR(&(packet->msg), packet->cmsg))
        {
            if (packet->cmsg->cmsg_level == IPPROTO_IP && packet->cmsg->cmsg_type == IP_TTL)
            {
                packet_stats->ttl = *(uint8_t *)CMSG_DATA(packet->cmsg);
                break;
            }
        }

        // Print the statistics of the received packet
        printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.1f ms\n", packet_stats->received_size, inet_ntoa(((struct sockaddr_in *)addr)->sin_addr), icmp.sequence, packet_stats->ttl, packet_stats->rtt);
        packet_stats->num_received++;
    }
}

void create_socket()
{
    // Create a raw socket with ICMP protocol, AF_INET is the address family for IPv4
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "%s: socket error in main\n", PROGRAM_NAME);
        exit_clean(sockfd, ERROR_SOCKET_OPEN);
    }
}

void set_packet_stats_structure(t_packet_stats *packet_stats)
{
    packet_stats->num_received = 0;
}

void set_packet_structure(t_packet *packet)
{
    struct iovec iov;

    char ancillary_buffer[MAX_PACKET_SIZE];

    packet->packet_size = sizeof(t_icmp_header);
    iov.iov_base = packet;
    iov.iov_len = packet->packet_size;

    packet->msg.msg_iov = &iov;
    packet->msg.msg_iovlen = 1;
    packet->msg.msg_control = ancillary_buffer;
    packet->msg.msg_controllen = sizeof(ancillary_buffer);
}

int main(int argc, char *argv[])
{
    int status;
    t_args args;
    t_packet_stats packet_stats;
    t_packet packet;
    struct hostent *he;
    struct in_addr ipv4_addr;
    struct sockaddr_in dest_addr;

    // The SIGINT signal is sent to a program when the user presses Ctrl+C, closing the program
    signal(SIGINT, int_handler);

    create_socket();
    set_args_structure(&args);
    set_packet_stats_structure(&packet_stats);
    set_packet_structure(&packet);
    parse_args(argc, argv, &args);

    status = inet_pton(AF_INET, args.host, &ipv4_addr);
    if (status == 0)
    { // Input is not an IPv4 address, try resolving as a domain name
        he = ft_gethostbyname(args.host);
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

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0; // Use default port
    dest_addr.sin_addr = ipv4_addr;
    for (int i = 0; args.num_packets < 0 || i < args.num_packets; i++)
    {
        printf("Sending ping to %s...\n", inet_ntoa(dest_addr.sin_addr)); // DEBUG
        send_ping(&args, (struct sockaddr *)&dest_addr, i);
        receive_ping(&args, &packet_stats, &packet, (struct sockaddr *)&dest_addr, i);
        sleep(1);
    }

    close(sockfd);
    return 0;
}