/*
** EPITECH PROJECT, 2023
** NeuralNetwork
** File description:
** NeuralNetwork
*/

#include "neuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(std::vector<int> config)
{
    _config = config;
    for (int i = 0 ; i < config.size() ; i++) {
        //initialize neurons
        _neurons.push_back(new Vector(config[i]));

        //initialize costs
        _costs.push_back(new Vector(config[i]));

        //initialize bias
        if (i != 0) {
            _bias.push_back(new Vector(config[i]));
            for (int j = 0 ; j < config[i] ; j++)
                _bias.back()->coeffRef(j) = 1;
        }

        //initialize weights
        if (i > 0) {
            _weights.push_back(new Matrix(config[i], config[i - 1]));
            _weights.back()->setRandom();
        }
    }
    debugPrints();
}

void NeuralNetwork::propagateForward(Vector &input)
{
    _neurons.front() = &input;

    for (int i = 1 ; i < _config.size() ; i++) {
        *_neurons[i] = (*_neurons[i - 1]) * (*_weights[i - 1]) + (*_bias[i]);
    }
}

void NeuralNetwork::debugPrints()
{
    //count number total of neurons
    int totalNeurons = 0;
    for (int j = 0 ; j < _neurons.size() ; j++)
        totalNeurons += _neurons[j]->size();
    std::cout << "total neurons: " << totalNeurons << std::endl;

    //count number total of weights
    int totalWeights = 0;
    for (int j = 1 ; j < _weights.size() ; j++)
        for (int k = 0 ; k < _weights[j]->rows() ; k++)
            totalWeights += _weights[j - 1]->row(k).size();
    std::cout << "total weights: " << totalWeights << std::endl;
    
    //count number total of bias
    int totalBias = 0;
    for (int j = 0 ; j < _bias.size() ; j++)
        totalBias += _bias[j]->size();
    std::cout << "total bias: " << totalBias << std::endl;
}
