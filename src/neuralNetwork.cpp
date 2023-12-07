/*
** EPITECH PROJECT, 2023
** NeuralNetwork
** File description:
** NeuralNetwork
*/

#include "neuralNetwork.hpp"

#include <fstream>

NeuralNetwork::NeuralNetwork(std::vector<int> config, float learningRate)
{
    _learningRate = learningRate;
    _config = config;
    for (int i = 0 ; i < config.size() ; i++) {
        //initialize neurons
        _neurons.push_back(new Vector(config[i]));

        //initialize costs
        _costs.push_back(new Vector(_neurons.size()));
        _caches.push_back(new Vector(_neurons.size()));

        //initialize bias
        if (i != 0) {
            _bias.push_back(new Vector(config[i]));
            for (int j = 0 ; j < config[i] ; j++)
                _bias.back()->coeffRef(j) = 1;
        }

        //initialize weights
        if (i > 0) {
            _weights.push_back(new Matrix(config[i - 1], config[i]));
            _weights.back()->setRandom();
        }
    }
    debugPrints();
}

float activation(float x)
{
    return tanhf(x);
}

float activationFunctionDerivative(float x)
{
    return 1 - (tanhf(x) * tanhf(x));
}

void NeuralNetwork::propagateForward(Vector &input)
{
    (*_neurons.front()) = input;

    for (int i = 1 ; i < _neurons.size() ; i++) {
        *_neurons[i] = (*_neurons[i - 1]) * (*_weights[i - 1]);
        _neurons[i]->unaryExpr(std::function(activation));
    }
}

void NeuralNetwork::calculateCost(Vector &output)
{
    *_costs.back() = output - (*_neurons.back());

    for (int i = _costs.size() - 2 ; i > 0; i--) {
        (*_costs[i]) = (*_costs[i + 1]) * (_weights[i]->transpose());
    }
}

void NeuralNetwork::updateWeights()
{
    // for (int i = 0; i < _weights.size(); i++) {
    //     Matrix weightUpdate = _learningRate * (*_neurons[i]).transpose() * (*_costs[i + 1]);

    //     // Gradient clipping (adjust  threshold as needed)
    //     double threshold = 1.0;
    //     weightUpdate = weightUpdate.array().max(-threshold).min(threshold);

    //     *_weights[i] -= weightUpdate;
    // }

    std::cout << "update weights" << std::endl;

    for (uint i = 0; i < _config.size() - 1; i++) {
        for (uint c = 0; c < _weights[i]->cols(); c++) {
            for (uint r = 0; r < _weights[i]->rows(); r++) {
                // Calcul du gradient
                double gradient = _learningRate * _costs[i + 1]->coeffRef(c) * activationFunctionDerivative(_caches[i + 1]->coeffRef(c)) * _neurons[i]->coeffRef(r);
                
                // Gradient clipping
                double threshold = 1.0;
                gradient = std::max(-threshold, std::min(gradient, threshold));

                // Mise à jour des poids
                _weights[i]->coeffRef(r, c) += gradient;
            }
        }
    }
    std::cout << "update bias" << std::endl;
}

void NeuralNetwork::backPropagation(Vector &expected)
{
    calculateCost(expected);
    updateWeights();
}

void NeuralNetwork::debugPrints()
{
    //count number total of neurons+ (*_bias[i - 1])
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

void NeuralNetwork::train(std::vector<Vector> &input, std::vector<Vector> &output)
{
    for (int i = 0 ; i < input.size() ; i++) {
        propagateForward(input[i]);
        backPropagation(output[i]);
    }
}

void NeuralNetwork::saveToFile(std::string filename)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0 ; i < _weights.size() ; i++) {
            for (int j = 0 ; j < _weights[i]->rows() ; j++) {
                for (int k = 0 ; k < _weights[i]->row(j).size() ; k++) {
                    file << _weights[i]->row(j)[k] << " ";
                }
                file << std::endl;
            }
            file << std::endl;
        }

        for (int i = 0 ; i < _bias.size() ; i++) {
            for (int j = 0 ; j < _bias[i]->size() ; j++) {
                file << _bias[i]->coeffRef(j) << " ";
            }
            file << std::endl;
        }

    }
}

void NeuralNetwork::loadFromFile(std::string filneame)
{
    std::ifstream file(filneame);
    if (file.is_open()) {
        for (int i = 0 ; i < _weights.size() ; i++) {
            for (int j = 0 ; j < _weights[i]->rows() ; j++) {
                for (int k = 0 ; k < _weights[i]->row(j).size() ; k++) {
                    file >> _weights[i]->row(j)[k];
                }
            }
        }

        for (int i = 0 ; i < _bias.size() ; i++) {
            for (int j = 0 ; j < _bias[i]->size() ; j++) {
                file >> _bias[i]->coeffRef(j);
            }
        }
    }
}

Vector &NeuralNetwork::getOutput()
{
    return *_neurons.back();
}