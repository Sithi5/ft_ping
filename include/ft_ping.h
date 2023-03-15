#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdbool.h>

#define PACKET_SIZE 64
#define PROGRAM_NAME "ft_ping"

typedef struct s_struct
{
} t_s;

typedef struct s_args
{
    bool v_flag;
    int num_packets;
    bool h_flag;
    char *host;
} t_args;

void parse_args(int argc, char *argv[], t_args *args);
t_args *set_args_structure();

#endif
