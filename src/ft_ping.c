#include "ft_ping.h"

int sockfd = -1;

void int_handler(int signo)
{
    close(sockfd);
    exit(0);
}

void send_ping(t_args *args)
{
}

void receive_ping(t_args *args)
{
}

int main(int argc, char *argv[])
{
    t_args *args;

    args = set_args_structure();
    parse_args(argc, argv, args);

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("socket error");
        return 1;
    }

    struct hostent *target_host = gethostbyname(args->host);
    if (target_host == NULL)
    {
        printf("%s: cannot resolve %s: Unknown host\n", PROGRAM_NAME, args->host);
        return 1;
    }

    // The SIGINT signal is sent to a program when the user presses Ctrl+C, closing the program
    signal(SIGINT, int_handler);

    for (int i = 0; args->num_packets < 0 || i < args->num_packets; i++)
    {
        send_ping(args);
        receive_ping(args);
        sleep(1);
    }
    return 0;
}