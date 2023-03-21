#include "ft_ping.h"

/*
 * Function to calculate the checksum of an ICMP packet
 */
unsigned short ft_icmp_checksum(void *data, int len) {
    unsigned short *buf = (uint16_t *) data;
    unsigned int sum = 0;

    // Sum all 16-bit words in the buffer
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }

    // Add any odd byte
    if (len == 1) {
        sum += *(uint8_t *) buf;
    }

    // Add carry if any
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    // Take one's complement of sum
    return (uint16_t) (~sum);
}

/*
 * This function is used to calculate the round trip time of a package
 */
double calculate_package_rtt(struct timeval *sent_time, struct timeval *end_time) {
    return ((double) (end_time->tv_sec - sent_time->tv_sec) * 1000.0) +
           ((double) (end_time->tv_usec - sent_time->tv_usec) / 1000.0);
}

/*
 * This function is used to resolve the DNS name of a server given its IP address
 */
char *ft_reverse_dns_lookup(struct sockaddr *server_addr, size_t dns_name_len) {
    int status;
    static char dns_name[NI_MAXHOST];

    status = getnameinfo(server_addr, sizeof(struct sockaddr_in), dns_name, dns_name_len, NULL, 0,
                         NI_NAMEREQD);
    if (status != 0) {
        // If the DNS name cannot be resolved, the IP address is returned instead
        inet_ntop(AF_INET, &(((struct sockaddr_in *) server_addr)->sin_addr), dns_name,
                  dns_name_len);
    }

    return dns_name;
}

/*
 * This function is used to resolve the IP address of a server given its DNS name
 */
struct sockaddr_in *ft_gethostbyname(const char *name, int num_addrs) {
    struct addrinfo hints;
    struct addrinfo *result;
    static struct sockaddr_in ipv4[MAX_ADDRS];
    int status;

    // Set up hints for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // Allow IPv4 only
    hints.ai_socktype = SOCK_DGRAM;   // Use datagram sockets

    // Call getaddrinfo to resolve the hostname
    status = getaddrinfo(name, NULL, &hints, &result);
    if (status != 0) {
        return NULL;
    }

    // Initialize the number of addresses to 0
    num_addrs = 0;

    // Loop through the result list and store up to MAX_ADDRS addresses in the static array
    for (struct addrinfo *rp = result; rp != NULL && num_addrs < MAX_ADDRS; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {   // IPv4
            ft_memcpy(&(ipv4[num_addrs]), rp->ai_addr, sizeof(struct sockaddr_in));
            num_addrs++;
        }
    }

    // Clean up the result list
    freeaddrinfo(result);

    return ipv4;
}