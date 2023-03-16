#include "ft_ping.h"

void ft_perror(const char *message)
{
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
}