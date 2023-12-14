/*
** EPITECH PROJECT, 2023
** parser
** File description:
** parse
*/

#pragma once
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>

typedef struct Parsing {
    std::vector<int> newNetworkConfig;
    std::string loadFile;
    bool trainMode = false;
    bool predictMode = false;
    std::string saveFile;
    std::string chessboardsFile;
    std::string activationFunction = "tanhf";
    float learningRate = 0.005;
} Parsing_t;

Parsing_t parseArgs(int ac, char **av);
void usage();