#include "ft_ping.h"

void usage()
{
    printf("Usage\n"
           "  %s [options] <destination>\n\n"
           "Options:\n"
           "  <destination>      dns name or ip address\n"
           "  -h                 show this help message and exit\n"
           //    "  -D                 print timestamps\n"
           "  -i <interval>      seconds between sending each packet\n"
           "  -c <count>         stop after <count> replies\n",
           PROGRAM_NAME);
    exit(1);
}

void set_args_structure(t_args *args)
{
    args->v_flag = false;
    args->h_flag = false;
    args->interval = 1;
    args->host = NULL;
    args->num_packets = -1;
}

void parse_args(int argc, char *argv[], t_args *args)
{
    if (argc < 2)
    {
        usage();
    }
    for (int i = 1; i < argc; i++)
    {
        if (ft_strcmp(argv[i], "-v") == 0)
        {
            args->v_flag = 1;
        }
        else if (ft_strcmp(argv[i], "-c") == 0 && i + 1 < argc)
        {
            if (i + 1 < argc && ft_isnumber(argv[i + 1]))
            {
                args->num_packets = ft_atoi(argv[i + 1]);
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
                args->interval = ft_str_to_double(argv[i + 1]);
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
        else if (args->host == NULL)
        {
            if (argv[i][0] == '-')
            {
                fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
                usage();
            }
            args->host = argv[i];
        }
        else
        {
            fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
            usage();
        }
    }
    if (args->host == NULL)
    {
        usage();
        exit(ERROR_ARGS);
    }
}