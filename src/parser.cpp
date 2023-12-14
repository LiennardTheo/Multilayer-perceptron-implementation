/*
** EPITECH PROJECT, 2023
** parser
** File description:
** parsing
*/

#include "parser.hpp"

void usage()
{
    std::cout << "Usage: ./myTorch [...] = MANDATORY / () = Optional\n [-n IN_LAYER (HIDDEN_LAYERS...) OUT_LAYER | -l LOADFILE]\n (-t (to train the network) | -p (to check how the network is performing))\n (-s SAVEFILE)\n [FILE (File containing dataset)]\n (-r LEARNING_RATE (default 0.005))\n (-m ACTIVATION FUNCTION (either \"sigmoid\" or \"tanhf\"))" << std::endl;
    exit(0);
}

Parsing_t parseArgs(int ac, char **av)
{
    int opt;
    Parsing_t parsing;

    while ((opt = getopt(ac, av, "hn:l:tps:r:m:")) != -1) {
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
            case 'r':
                parsing.learningRate = std::stof(optarg);
                break;
            case 'm':
                parsing.activationFunction = optarg;
                break;
            default:
                usage();
        }
    }

    // Retrieve the non-option argument (chessboards file)
    if (parsing.trainMode || parsing.predictMode) {
        if (optind < ac)
            parsing.chessboardsFile = av[optind];
        else
            usage();
    } else {
        if (optind > ac)
            usage();
    }

    return parsing;
}
