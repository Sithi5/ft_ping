#include "ft_ping.h"

void usage()
{
    printf("Usage: ft_ping [-v] destination\n");
    exit(1);
}

void parse_args(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "vh")) != -1)
    {
        switch (opt)
        {
            case 'v':
            verbose = true;
            break;
            case 'h':
            usage();
            break;
            default:
            usage();
            break;
        }
    }
    if (argc - optind != 1)
    {
        usage();
    }
}


int main(int argc, char *argv[])
{
    pid = getpid();

    // The SIGINT signal is sent to a program when the user presses Ctrl+C, closing the program
    signal(SIGINT, int_handler);
    // The alarm signal is used to determine when a packet has timed out and no response has been received.
    signal(SIGALRM, icmp_handler);
    parse_args(argc, argv);

    return 0;
}