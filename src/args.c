#include "ft_ping.h"

void usage()
{
    printf("Usage\n"
           "  %s [options] <destination>\n\n"
           "Options:\n"
           "  <destination>      dns name or ip address\n"
           "  -h                 show this help message and exit\n"
           "  -a                 use audible ping\n"
           "  -D                 print timestamps\n"
           "  -i <interval>      seconds between sending each packet\n"
           "  -c <count>         stop after <count> replies\n",
           PROGRAM_NAME);
    exit(1);
}

void parse_args(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
    }
    for (int i = 1; i < argc; i++)
    {
        if (ft_strcmp(argv[i], "-v") == 0)
        {
            ping.args.v_flag = true;
        }
        else if (ft_strcmp(argv[i], "-D") == 0)
        {
            ping.args.D_flag = true;
        }
        else if (ft_strcmp(argv[i], "-a") == 0)
        {
            ping.args.a_flag = true;
        }
        else if (ft_strcmp(argv[i], "-c") == 0 && i + 1 < argc)
        {
            if (i + 1 < argc && ft_isnumber(argv[i + 1]))
            {
                ping.args.num_packets = ft_atoi(argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "%s: invalid count of packets to transmit: `%s'\n", PROGRAM_NAME, argv[i + 1]);
                exit(ERROR_ARGS);
            }
        }
        else if (ft_strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            if (i + 1 < argc && ft_isdouble(argv[i + 1]))
            {
                ping.args.interval = ft_str_to_double(argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "%s: invalid interval: `%s'\n", PROGRAM_NAME, argv[i + 1]);
                exit(ERROR_ARGS);
            }
        }
        else if (ft_strcmp(argv[i], "-h") == 0)
        {
            usage();
        }
        else if (argv[i][0] == '-')
        {
            fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
            usage();
        }
        else if (ping.args.host == NULL)
        {
            ping.args.host = argv[i];
        }
        else
        {
            fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
            usage();
        }
    }
    if (ping.args.host == NULL)
    {
        usage();
        exit(ERROR_ARGS);
    }
}