#include "ft_ping.h"

void icmp_handler()
{
}

void int_handler(int signo)
{
    close(sockfd);
    exit(0);
}
