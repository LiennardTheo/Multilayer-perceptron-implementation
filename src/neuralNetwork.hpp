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

typedef Eigen::VectorXf Vector;
typedef Eigen::MatrixXf Matrix;

class NeuralNetwork {
    public:
        NeuralNetwork(std::vector<int> config);
        ~NeuralNetwork() = default;

        void propagateForward(Vector &input);

        void propagateBackward(Vector &output);

        void calculateCost(Vector &output);

        void updateWeights();

        void train(Vector &input, Vector &output);

        void debugPrints();

    private:
        std::vector<Vector *> _neurons;
        std::vector<Matrix *> _weights;
        std::vector<Vector *> _bias;
        std::vector<Vector *> _costs;
        std::vector<int> _config;
};

