#ifndef PING_H
#define PING_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#ifdef TEST

#define PROGRAM_NAME "ping"

#else

#define PROGRAM_NAME "ft_ping"

#endif

#define PACKET_SIZE 64
#define MAX_PACKET_SIZE 1024
#define MAX_ADDRS 16

/****************************************************************************/
/*                           ENUM                                           */
/****************************************************************************/

/* Error codes */
enum e_error
{
    ERROR_SOCKET_OPEN = 1,
    ERROR_GET_HOST_BY_NAME_SOCKET_OPEN,
    ERROR_SENDTO,
    ERROR_INET_PTON,
    ERROR_RESOLVING_HOST,
    ERROR_RECVFROM,
    NB_OF_ERROR_CODES /* Always keep last */
};

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_struct
{
} t_s;

typedef struct s_icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
    struct timeval timestamp;
} t_icmp_header;

typedef struct s_args
{
    bool v_flag;
    int num_packets;
    bool h_flag;
    char *host;
} t_args;

typedef struct s_packet_stats
{
    int num_received;
    u_int8_t ttl;
    double rtt;
    int received_size;
    struct timeval end_time;
} t_packet_stats;

typedef struct s_packet
{
    struct msghdr msg;
    struct cmsghdr *cmsg;
    char packet[MAX_PACKET_SIZE];
    int packet_size;
} t_packet;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// errors
void ft_perror(const char *message);

// args
void parse_args(int argc, char *argv[], t_args *args);
void set_args_structure();

// network
unsigned short ft_icmp_checksum(void *data, int len);
struct hostent *ft_gethostbyname(const char *name);

// utils
void ft_bzero(void *s, size_t n);
void *ft_memset(void *s, int c, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);

#endif
