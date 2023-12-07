/*
** EPITECH PROJECT, 2023
** main
** File description:
** main test
*/

#include "neuralNetwork.hpp"

#include <fstream>
#include <sstream>

void parseFEN(const std::string& fen, Vector& inputVec) {
    int index = 0;
    int row = 0, col = 0; // Start at the top-left corner of the board (8th row, 1st column)

    for (char c : fen) {
        if (c == ' ') {
            break; // FEN parsing complete
        } else if (c == '/') {
            // Move to the next row
            row++;
            col = 0;
        } else if (isdigit(c)) {
            // Empty squares represented by a number indicating the count of empty squares
            int count = c - '0';
            for (int i = 0; i < count; ++i) {
                inputVec[row * 8 + col] = 0; // 0 represents an empty square
                col++;
            }
        } else {
            // Piece notation
            int piece;
            switch (c) {
                case 'r':
                    piece = 7;
                    break;
                case 'R':
                    piece = 1; // Piece code for a rook
                    break;
                case 'n':
                    piece = 8;
                    break;
                case 'N':
                    piece = 2; // Piece code for a knight
                    break;
                case 'b':
                    piece = 9;
                    break;
                case 'B':
                    piece = 3; // Piece code for a bishop
                    break;
                case 'q':
                    piece = 10;
                    break;
                case 'Q':
                    piece = 4; // Piece code for a queen
                    break;
                case 'k':
                    piece = 11;
                    break;
                case 'K':
                    piece = 5; // Piece code for a king
                    break;
                case 'p':
                    piece = 12;
                    break;
                case 'P':
                    piece = 6; // Piece code for a pawn
                    break;
                default:
                    piece = 0; // Unknown piece or empty square
                    break;
            }
            inputVec[row * 8 + col] = piece;
            col++;
        }
    }
    //normalize Vector
    for (int i = 0 ; i < inputVec.size() ; i++) {
        inputVec[i] /= 12;
    }

}

void parseChessFile(const std::string& filename, int trainingSteps, int examplesPerStep,
                    std::vector<Vector>& input, std::vector<Vector>& output) {
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string token;

    while (std::getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        if (line.find("RES") != std::string::npos) {
            std::getline(file, line); // Move to CHECKMATE line

            std::istringstream iss(line);
            iss >> token >> token; // Read the CHECKMATE token

            bool checkmate;
            if (token == "True")
                checkmate = true;
            else if (token == "False")
                checkmate = false;
            else
                continue; // Skip this example

            // Process the FEN and board representation

            Vector inputVec(64);
            std::getline(file, line); // Read FEN line
            std::istringstream iss2(line);
            iss2 >> token >> token; // FEN
            parseFEN(token, inputVec);

            for (int i = 0; i < 10; i++) {
                std::getline(file, line);
            }

            input.push_back(inputVec);

            Vector outputVec(1);
            outputVec << (checkmate ? 1.0f : 0.0f);
            output.push_back(outputVec);

            if (input.size() >= trainingSteps * examplesPerStep) {
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

void exploit(NeuralNetwork &test, std::vector<Vector> &input, std::vector<Vector> &output)
{
    int count = 0;
    for (int i = 0 ; i < input.size() ; i++) {
        test.propagateForward(input[i]);
        if (test.getOutput()[0] > 0.5 && output[i][0] == 1)
            count++;
        else if (test.getOutput()[0] < 0.5 && output[i][0] == 0)
            count++;
        else 
            std::cout << "output: " << test.getOutput()[0] << " expected: " << output[i][0] << std::endl;
    }
    std::cout << "exploit: " << count << "/" << input.size() << std::endl;
}

void shuffle(std::vector<Vector> &input, std::vector<Vector> &output)
{
    std::vector<Vector> tmpInput;
    std::vector<Vector> tmpOutput;
    int random;
    while (input.size() > 0) {
        random = rand() % input.size();
        tmpInput.push_back(input[random]);
        tmpOutput.push_back(output[random]);
        input.erase(input.begin() + random);
        output.erase(output.begin() + random);
    }
    input = tmpInput;
    output = tmpOutput;
}

int main()
{
    std::vector<int> config = {64, 32, 16, 1};
    NeuralNetwork test(config);
    std::vector<Vector> input;
    std::vector<Vector> output;
    parseChessFile("DataSets/datasets/checkmate/10_pieces.txt", 1, 100000, input, output);
    parseChessFile("DataSets/datasets/checkmate/20_pieces.txt", 1, 100000, input, output);
    parseChessFile("DataSets/datasets/boards/10_pieces.txt", 1, 100000, input, output);
    parseChessFile("DataSets/datasets/boards/lots_pieces.txt", 1, 100000, input, output);
    parseChessFile("DataSets/datasets/checkmate/lots_pieces.txt", 1, 105000, input, output);
    parseChessFile("DataSets/datasets/boards/20_pieces.txt", 1, 100000, input, output);

    shuffle(input, output);

    test.train(input, output);

    shuffle(input, output);
    exploit(test, input, output);
    test.saveToFile("test.txt");
    return 0;
}
