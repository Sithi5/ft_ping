#include "ft_ping.h"

/* Function to calculate the checksum of an ICMP packet
 * @param data The ICMP packet
 * @param len The length of the ICMP packet
 * @return The checksum of the ICMP packet
 */
unsigned short ft_icmp_checksum(void *data, int len)
{
    unsigned short *buf = (uint16_t *)data;
    unsigned int sum = 0;

    // Sum all 16-bit words in the buffer
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }

    // Add any odd byte
    if (len == 1)
    {
        sum += *(uint8_t *)buf;
    }

    // Add carry if any
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    // Take one's complement of sum
    return (uint16_t)(~sum);
}

/**
 * Function to resolve a hostname to an IP address
 * @param name The hostname to resolve
 * @return A pointer to a hostent struct containing the IP address
 */
struct hostent *ft_gethostbyname(const char *name)
{
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "%s: socket error in ft_gethostname", PROGRAM_NAME);
        exit(ERROR_SOCKET_OPEN);
    }

    // Set up hints for getaddrinfo
    struct addrinfo hints;
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;      // Allow IPv4 only
    hints.ai_socktype = SOCK_DGRAM; // Use datagram sockets

    // Call getaddrinfo to resolve the hostname
    struct addrinfo *result;
    int status = getaddrinfo(name, NULL, &hints, &result);
    if (status != 0)
    {
        return NULL;
    }

    // Loop through the result list and store up to MAX_ADDRS addresses in a static array because we can't use malloc
    static struct in_addr addr_list[MAX_ADDRS];
    int num_addrs = 0;
    for (struct addrinfo *rp = result; rp != NULL && num_addrs < MAX_ADDRS; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            memcpy(&addr_list[num_addrs], &(ipv4->sin_addr), sizeof(struct in_addr));
            num_addrs++;
        }
    }

    // Clean up the result list
    freeaddrinfo(result);

    // Construct the hostent struct manually
    static struct hostent he;
    static char *aliases[] = {NULL};
    static char addr_ptrs[MAX_ADDRS * sizeof(char *)];
    static char addr_list_buf[MAX_ADDRS * sizeof(struct in_addr)];
    he.h_name = (char *)name;
    he.h_aliases = aliases;
    he.h_addrtype = AF_INET;
    he.h_length = sizeof(struct in_addr);
    he.h_addr_list = (char **)addr_ptrs;
    for (int i = 0; i < num_addrs; i++)
    {
        he.h_addr_list[i] = &addr_list_buf[i * sizeof(struct in_addr)];
        memcpy(he.h_addr_list[i], &addr_list[i], sizeof(struct in_addr));
    }
    he.h_addr_list[num_addrs] = NULL;

    // Close the socket
    close(sockfd);

    return &he;
}
