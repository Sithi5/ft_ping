#include "ft_ping.h"

t_args *set_args_structure()
{
    t_args *args = malloc(sizeof(t_args));
    if (args == NULL)
    {
        perror("args structure malloc");
        exit(1);
    }
    args->v_flag = false;
    args->h_flag = false;
    args->host = NULL;
    args->num_packets = -1;
    return args;
}