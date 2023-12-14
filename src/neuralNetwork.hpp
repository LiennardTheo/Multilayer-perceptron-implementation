/*
** EPITECH PROJECT, 2023
** neural Networks
** File description:
** ye
*/

#pragma once

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Eigen>

typedef Eigen::RowVectorXf Vector;
typedef Eigen::MatrixXf Matrix;

class NeuralNetwork {
    public:
        NeuralNetwork(std::vector<int> config, std::string activation,  float learningRate = 0.005f);
        ~NeuralNetwork() = default;

        void propagateForward(Vector &input);

        void backPropagation(Vector &expected);

        void calculateCost(Vector &output);

        void updateWeights();

        void train(std::vector<Vector> &input, std::vector<Vector> &output);

        void saveToFile(std::string filename);

        void loadFromFile(std::string filename);

        void debugPrints();

        Vector &getOutput();

    private:
        std::vector<Vector *> _neurons;
        std::vector<Matrix *> _weights;
        std::vector<Vector *> _bias;
        std::vector<Vector *> _costs;
        std::vector<Vector *> _caches;
        std::vector<int> _config;
        std::function<float(float)> _activationFunction;
        std::function<float(float)> _activationFunctionDerivative;
        float _learningRate;
};

