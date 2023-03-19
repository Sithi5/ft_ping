#ifndef PING_H
#define PING_H

/****************************************************************************/
/*                          INCLUDES                                        */
/****************************************************************************/

#include <arpa/inet.h>
#include <errno.h>
#include <float.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/****************************************************************************/
/*                          DEFINES                                         */
/****************************************************************************/

#define PROGRAM_NAME    "ft_ping"
#define PROGRAM_VERSION "1.0.0"
#define PROGRAM_AUTHOR  "mabouce"

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#define PACKET_SIZE     64
#define MAX_PACKET_SIZE 1024
#define MAX_ADDRS       16
#define DEFAULT_TTL     64

/****************************************************************************/
/*                           ENUM                                           */
/****************************************************************************/

/* Error codes */
enum e_error {
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

typedef struct s_args {
    bool a_flag;
    bool D_flag;
    bool h_flag;
    bool i_flag;
    bool n_flag;
    bool q_flag;
    bool v_flag;
    bool w_flag;
    int deadline;
    double interval;
    int num_packets;
    char *host;
} t_args;

typedef struct s_packets_stats {
    int transmitted;
    int received;
    double min_rtt;
    double max_rtt;
    double sum_rtt;
    double sum_squared_rtt;
} t_packets_stats;

typedef struct s_ping {
    t_packets_stats packets_stats;
    t_args args;
    int sockfd;
} g_ping;

/****************************************************************************/
/*                          GLOBALS                                         */
/****************************************************************************/

extern g_ping ping;

/****************************************************************************/
/*                          FUNCTIONS DEFINITIONS                           */
/****************************************************************************/

// prints
void print_ping_address_infos(struct sockaddr_in *server_addr);
void print_statistics();

// struct
void set_packets_stats();
void set_args_structure();

// send_packages
int send_ping(struct sockaddr_in server_addr, int sequence);

// receive_packages
void receive_ping(struct sockaddr *addr, int sequence);

// errors
void ft_perror(const char *message);
void exit_clean(int sockfd, int status);

// args
void parse_args(int argc, char *argv[]);

// network
unsigned short ft_icmp_checksum(void *data, int len);
struct sockaddr_in *ft_gethostbyname(const char *name, int num_addrs);
double calculate_package_rtt(struct timeval *sent_time, struct timeval *end_time);
char *ft_reverse_dns_lookup(struct sockaddr *server_addr, size_t dns_name_len);

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
double ft_str_to_double(const char *str);
bool ft_isdouble(const char *str);

#endif
