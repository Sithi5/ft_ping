#include "ft_ping.h"

void print_ping_address_infos(t_args *args, struct sockaddr *server_addr)
{
    char ip_address[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(((struct sockaddr_in *)server_addr)->sin_addr), ip_address, INET_ADDRSTRLEN);
    printf("PING %s (%s): %lu data bytes\n",
           args->host,
           ip_address,
           sizeof(t_packet));
}