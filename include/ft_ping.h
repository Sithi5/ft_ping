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
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <float.h>

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#define PROGRAM_NAME "ft_ping"

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
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
    ERROR_ARGS,
    ERROR_SOCKET_OPTION,
    ERROR_GET_HOST_BY_NAME_SOCKET_OPEN,
    ERROR_SENDTO,
    ERROR_INET_PTON,
    ERROR_RESOLVING_HOST,
    ERROR_RECVFROM,
    ERROR_SIGINT,
    NB_OF_ERROR_CODES /* Always keep last */
};

/****************************************************************************/
/*                          STRUCTS                                         */
/****************************************************************************/

typedef struct s_packet
{
    struct ip ip;
    struct icmp icmp;
} t_packet;

typedef struct s_args
{
    bool v_flag;
    int num_packets;
    bool h_flag;
    char *host;
} t_args;

typedef struct s_packets_stats
{
    int transmitted;
    int received;
    double min_rtt;
    double max_rtt;
    double sum_rtt;
    double sum_squared_rtt;
} t_packets_stats;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// prints
void print_ping_address_infos(t_args *args, struct sockaddr *server_addr);

// stats
void print_statistics(t_packets_stats *packets_stats, const char *hostname);
void set_packets_stats(t_packets_stats *packets_stats);

// send_packages
int send_ping(int sockfd, t_args *args, struct sockaddr_in server_addr, uint16_t sequence, t_packets_stats *packets_stats);

// receive_packages
void receive_ping(int sockfd, t_args *args, t_packets_stats *packets_stats, struct sockaddr *addr, uint16_t sequence);

// errors
void ft_perror(const char *message);
void exit_clean(int sockfd, int status);

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
double ft_sqrt(double num);
double ft_fab(double num, int power);
bool ft_isdigit(char c);
int ft_isnumber(const char *str);
int ft_atoi(const char *str);
int ft_strcmp(const char *s1, const char *s2);

#endif
