#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>
#include <errno.h>

#define PACKET_SIZE 64
#define MAX_PACKET_SIZE 1024
#define PROGRAM_NAME "ft_ping"

#define MAX_ADDRS 16

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

// args
void parse_args(int argc, char *argv[], t_args *args);
void set_args_structure();

// network
unsigned short ft_icmp_checksum(void *data, int len);
struct hostent *ft_gethostbyname(const char *name);

// utils
void ft_perror(const char *message);
void ft_bzero(void *s, size_t n);

#endif
