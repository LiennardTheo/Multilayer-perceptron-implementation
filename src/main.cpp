/*
** EPITECH PROJECT, 2023
** main
** File description:
** main test
*/

#include "neuralNetwork.hpp"
#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <random>

void findBest(std::vector<Vector> &input, std::vector<Vector> &output);

void parseFEN(const std::string& fen, Vector& inputVec)
{
    int index = 0;
    int row = 0, col = 0;

    for (char c : fen) {
        if (c == ' ') {
            break;
        } else if (c == '/') {
            row++;
            col = 0;
        } else if (isdigit(c)) {
            int count = c - '0';
            for (int i = 0; i < count; ++i) {
                inputVec[row * 8 + col] = 0;
                col++;
            }
        } else {
            int piece;
            switch (c) {
                case 'r':
                    piece = 7;
                    break;
                case 'R':
                    piece = 1;
                    break;
                case 'n':
                    piece = 8;
                    break;
                case 'N':
                    piece = 2;
                    break;
                case 'b':
                    piece = 9;
                    break;
                case 'B':
                    piece = 3;
                    break;
                case 'q':
                    piece = 10;
                    break;
                case 'Q':
                    piece = 4;
                    break;
                case 'k':
                    piece = 11;
                    break;
                case 'K':
                    piece = 5;
                    break;
                case 'p':
                    piece = 12;
                    break;
                case 'P':
                    piece = 6;
                    break;
                default:
                    piece = 0;
                    break;
            }
            inputVec[row * 8 + col] = piece;
            col++;
        }
    }

    for (int i = 0 ; i < inputVec.size() ; i++) {
        inputVec[i] /= 12;
    }

}

void parseChessFile(const std::string& filename, int trainingSteps, int examplesPerStep,
                    std::vector<Vector>& input, std::vector<Vector>& output)
{
    int totalExamples = trainingSteps * examplesPerStep;
    int collectedExamples = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string token;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("RES") != std::string::npos) {
            std::istringstream izz(line);
            izz >> token >> token;
            bool pat;
            if (token == "1/2-1/2")
                pat = true;
            else
                pat = false;

            std::getline(file, line);

            std::istringstream iss(line);
            iss >> token >> token;

            bool checkmate;
            if (token == "True")
                checkmate = true;
            else if (token == "False")
                checkmate = false;
            else
                continue;

            Vector inputVec(64);
            std::getline(file, line);
            std::istringstream iss2(line);
            iss2 >> token >> token;
            parseFEN(token, inputVec);

            for (int i = 0; i < 9; i++) {
                std::getline(file, line);
            }

            input.push_back(inputVec);

            Vector outputVec(3);
            outputVec[0] = (checkmate ? 1.0f : 0.0f);
            outputVec[1] = (pat ? 1.0f : 0.0f);
            outputVec[2] = (!checkmate && !pat ? 1.0f : 0.0f);
            output.push_back(outputVec);

            collectedExamples++;
            if (collectedExamples >= totalExamples) {
                return;
            }
        }
    }

    file.close();
}

void printBoard(Vector &input, Vector &output)
{
    for (int i = 0 ; i < input.size() ; i++) {
        std::cout << input[i] << " ";
        if (i % 8 == 7)
            std::cout << std::endl;
    }
    for (int i = 0 ; i < output.size() ; i++) {
        std::cout << output[i] << std::endl;
    }
    std::cout << std::endl;
}

int exploit(NeuralNetwork &test, std::vector<Vector> &input, std::vector<Vector> &output)
{
    int count = 0;
    int checkmates = 0;
    int boards = 0;
    int localCount = 0;
    for (int i = 0 ; i < input.size() ; i++) {
        test.propagateForward(input[i]);
        for (int j = 0 ; j < output[i].size() ; j++) {
            if (test.getOutput()[j] > 0.5 && output[i][j] == 1)
                localCount++;
            else if (test.getOutput()[j] < 0.5 && output[i][j] == 0)
                localCount++;
        }
        if (localCount == output[i].size())
            count++;
        localCount = 0;
        if (output[i][0] == 1)
            checkmates++;
        else
            boards++;
    }
    std::cout << "exploit: " << count << "/" << input.size() << std::endl;
    return count;
}

void shuffle(std::vector<Vector> &input, std::vector<Vector> &output) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int n = input.size();
    for (int i = n - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(gen);
        std::swap(input[i], input[j]);
        std::swap(output[i], output[j]);
    }
}

void process(NeuralNetwork &network, Parsing_t &parsing)
{
    std::vector<Vector> input;
    std::vector<Vector> output;

    if (parsing.predictMode || parsing.trainMode) {
        for (int i = 0 ; i < parsing.chessboardsFile.size() ; i++) {
            std::cout << "Parsing chessboards file :" << parsing.chessboardsFile[i] << "..." << std::endl;
            parseChessFile(parsing.chessboardsFile[i], 1, parsing.DataSize, input, output);
        }
        std::cout << "Parsing done" << std::endl;
        std::cout << "Shuffling..." << std::endl;
        shuffle(input, output);
        std::cout << "Shuffling done" << std::endl;
        findBest(input, output);
        exit(0); findBest(input, output);

        if (parsing.trainMode) {
            std::cout << "Training..." << std::endl;
            // network.train(input, output);
            std::cout << "Training done" << std::endl;
        } else if (parsing.predictMode) {
            exploit(network, input, output);
        }
    }

    if (!parsing.saveFile.empty()) {
        std::cout << "Saving network to file: " << parsing.saveFile << std::endl;
        network.saveToFile(parsing.saveFile);
    }
}

int main(int ac, char **av)
{
    Parsing_t parsing = parseArgs(ac, av);

    if (parsing.newNetworkConfig.empty() && parsing.loadFile.empty()) {
        std::cerr << "Error : You must specify either -n (new network) or -l (load network)" << std::endl;
        usage();
    }

    std::vector<int> config = {64, 1};
    if (!parsing.newNetworkConfig.empty()) {
        config = parsing.newNetworkConfig;
        std::cout << "Creating new network with config: ";
        for (int i = 0 ; i < config.size() ; i++) {
            std::cout << config[i] << " ";
        }
        std::cout << std::endl;
    }

    if (parsing.activationFunction == "sigmoid") {
        std::cout << "Using sigmoid activation function" << std::endl;
    } else if (parsing.activationFunction == "tanhf") {
        std::cout << "Using tanhf activation function" << std::endl;
    } else if (parsing.activationFunction == "relu") {
        std::cout << "Using relu activation function" << std::endl;
    } else {
        std::cerr << "Error : Activation function not supported" << std::endl;
        usage();
    }

    NeuralNetwork network(config, parsing.activationFunction, parsing.learningRate);

    if (!parsing.loadFile.empty()) {
        network.loadFromFile(parsing.loadFile);
    }

    process(network, parsing);

    return 0;
}

void findBest(std::vector<Vector> &input, std::vector<Vector> &output)
{
    int max = 0;
    for (int i = 16 ; i < 64 ; i++) {
        for (int j = 16 ; j < 64 ; j++) {
            for (float lr = 0.001 ; lr < 0.01 ; lr += 0.001) {
                NeuralNetwork network({64, i, j, 3}, "tanhf", lr);
                network.train(input, output);
                shuffle(input, output);
                int count = exploit(network, input, output);
                if (count > max) {
                    max = count;
                    std::cout << "new max: " << max << std::endl;
                    std::cout << "i: " << i << std::endl;
                    std::cout << "j: " << j << std::endl;
                    std::cout << "lr: " << lr << std::endl;
                }
            }
        }
    }
}
