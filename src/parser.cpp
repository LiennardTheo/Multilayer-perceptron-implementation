/*
** EPITECH PROJECT, 2023
** parser
** File description:
** parsing
*/

#include "parser.hpp"

void usage()
{
    std::cout << "Usage: ./myTorch [...] = MANDATORY / () = Optional\n [-n IN_LAYER [HIDDEN_LAYERS...] OUT_LAYER | -l LOADFILE]\n [-t (to train the network) | -p (to check how the network is performing)]\n (-s SAVEFILE)\n [FILE (File containing dataset)]\n";
    exit(0);
}

Parsing_t parseArgs(int ac, char **av)
{
    int opt;
    Parsing_t parsing;

    while ((opt = getopt(ac, av, "hn:l:tps:")) != -1) {
        switch (opt) {
            case 'h':
                usage();
                break;
            case 'n':
                optind--;
                while (optind < ac && *av[optind] != '-') {
                    parsing.newNetworkConfig.push_back(std::stoi(av[optind]));
                    optind++;
                }
                break;
            case 'l':
                parsing.loadFile = optarg;
                break;
            case 't':
                parsing.trainMode = true;
                break;
            case 'p':
                parsing.predictMode = true;
                break;
            case 's':
                parsing.saveFile = optarg;
                break;
            default:
                usage();
        }
    }

    // Retrieve the non-option argument (chessboards file)
    if (optind < ac) {
        parsing.chessboardsFile = av[optind];
    } else {
        usage();
    }

    return parsing;
}
