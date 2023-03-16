#include "ft_ping.h"

void usage()
{
    printf("usage: %s [-hv] [-c count] host\n", PROGRAM_NAME);
    exit(1);
}

void set_args_structure(t_args *args)
{
    args->v_flag = false;
    args->h_flag = false;
    args->host = NULL;
    args->num_packets = -1;
    return args;
}
void parse_args(int argc, char *argv[], t_args *args)
{
    if (argc < 2)
    {
        usage();
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            args->v_flag = 1;
        }
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
        {
            args->num_packets = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            usage();
        }

        else if (args->host == NULL)
        {
            if (argv[i][0] == '-')
            {
                printf("Invalid option -- %s\n", argv[i]);
                usage();
            }
            args->host = argv[i];
        }
    }
}