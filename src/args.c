#include "ft_ping.h"

void usage() {
    printf("Usage\n"
           "  %s [options] <destination>\n\n"
           "Options:\n"
           "  <destination>      dns name or ip address\n"
           "  -a                 use audible ping\n"
           "  -c <count>         stop after <count> replies\n"
           "  -D                 print timestamps\n"
           "  -h                 show this help message and exit\n"
           "  -i <interval>      seconds between sending each packet\n"
           "  -n                 no dns name resolution"
           "  -q                 quiet output"
           "  -t <ttl>           define time to live"
           "  -v                 verbose output"
           "  -V                 print version and exit",
           PROGRAM_NAME);
    exit(1);
}

void parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
    }
    for (int i = 1; i < argc; i++) {
        if (ft_strcmp(argv[i], "-a") == 0) {
            ping.args.a_flag = true;
        } else if (ft_strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            if (i + 1 < argc && ft_isnumber(argv[i + 1])) {
                ping.args.num_packets = ft_atoi(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "%s: invalid count of packets to transmit: '%s'\n", PROGRAM_NAME,
                        argv[i + 1]);
                exit(ERROR_ARGS);
            }
        } else if (ft_strcmp(argv[i], "-D") == 0) {
            ping.args.D_flag = true;
        } else if (ft_strcmp(argv[i], "-h") == 0) {
            usage();
        } else if (ft_strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            if (i + 1 < argc && ft_isdouble(argv[i + 1])) {
                ping.args.i_flag = true;
                ping.args.interval = ft_str_to_double(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "%s: invalid interval: `%s'\n", PROGRAM_NAME, argv[i + 1]);
                exit(ERROR_ARGS);
            }
        } else if (ft_strcmp(argv[i], "-n") == 0) {
            ping.args.n_flag = true;
        } else if (ft_strcmp(argv[i], "-q") == 0) {
            ping.args.q_flag = true;
        } else if (ft_strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            if (i + 1 < argc && ft_isnumber(argv[i + 1])) {
                ping.args.ttl = ft_atoi(argv[i + 1]);
                if (ping.args.ttl < 0 || ping.args.ttl > 255) {
                    fprintf(stderr, "%s: invalid argument: '%s': out of range 0 <= value <= 255\n",
                            PROGRAM_NAME, argv[i + 1]);
                    exit(ERROR_ARGS);
                }
                i++;
            } else {
                fprintf(stderr, "%s: invalid argument: '%s'\n", PROGRAM_NAME, argv[i + 1]);
                exit(ERROR_ARGS);
            }
        } else if (ft_strcmp(argv[i], "-v") == 0) {
            ping.args.v_flag = true;
        } else if (ft_strcmp(argv[i], "-V") == 0) {
            printf("%s by %s %s\n", PROGRAM_NAME, PROGRAM_AUTHOR, PROGRAM_VERSION);
            exit(ERROR_ARGS);
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
            usage();
        } else if (ping.args.host == NULL) {
            ping.args.host = argv[i];
        } else {
            fprintf(stderr, "%s: Invalid option -- `%s`\n", PROGRAM_NAME, argv[i]);
            usage();
        }
    }
    if (ping.args.host == NULL) {
        usage();
        exit(ERROR_ARGS);
    }
}