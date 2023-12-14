/*
** EPITECH PROJECT, 2023
** NeuralNetwork
** File description:
** NeuralNetwork
*/

#include "neuralNetwork.hpp"

#include <fstream>

float tanhf_activation(float x)
{
    return tanhf(x);
}

float sigmoid(float x)
{
    return 1 / (1 + exp(-x));
}

float tanhf_activationFunctionDerivative(float x)
{
    return 1 - (tanhf(x) * tanhf(x));
}

float sigmoid_activationFunctionDerivative(float x)
{
    return x * (1 - x);
}

NeuralNetwork::NeuralNetwork(std::vector<int> config, std::string activation, float learningRate)
{
    _learningRate = learningRate;
    std::cout << "learning rate: " << _learningRate << std::endl;
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

    //initialize activation function
    if (activation == "tanhf") {
        _activationFunction = std::function<float(float)>(tanhf_activation);
        _activationFunctionDerivative = std::function<float(float)>(tanhf_activationFunctionDerivative);
    } else if (activation == "sigmoid") {
        _activationFunction = std::function<float(float)>(sigmoid);
        _activationFunctionDerivative = std::function<float(float)>(sigmoid_activationFunctionDerivative);
    } else {
        std::cerr << "Invalid activation function" << std::endl;
        exit(84);
    }

    debugPrints();
}

void NeuralNetwork::propagateForward(Vector &input)
{
    (*_neurons.front()) = input;

    for (int i = 1 ; i < _neurons.size() - 1 ; i++) {
        *_neurons[i] = (*_neurons[i - 1]) * (*_weights[i - 1]);
        *_neurons[i] =  _neurons[i]->unaryExpr(_activationFunction);
    }

    *_neurons.back() = (*_neurons[_neurons.size() - 2]) * (*_weights[_weights.size() - 1]);
    *_neurons.back() = _neurons.back()->unaryExpr(std::function<float(float)>(sigmoid));
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

    // Update weights between last hidden layer and output layer using sigmoid activation function derivative
    for (uint c = 0; c < _weights.back()->cols(); c++) {
        for (uint r = 0; r < _weights.back()->rows(); r++) {
            // Calcul du gradient
            double gradient = _learningRate * _costs.back()->coeffRef(c) * sigmoid_activationFunctionDerivative(_caches.back()->coeffRef(c)) * _neurons[_neurons.size() - 2]->coeffRef(r);
            
            // Gradient clipping
            double threshold = 1.0;
            gradient = std::max(-threshold, std::min(gradient, threshold));

            // Mise à jour des poids
            _weights.back()->coeffRef(r, c) += gradient;
        }
    }

    for (uint i = 0; i < _config.size() - 1; i++) {
        for (uint c = 0; c < _weights[i]->cols(); c++) {
            for (uint r = 0; r < _weights[i]->rows(); r++) {
                // Calcul du gradient
                double gradient = _learningRate * _costs[i + 1]->coeffRef(c) * _activationFunctionDerivative(_caches[i + 1]->coeffRef(c)) * _neurons[i]->coeffRef(r);
                
                // Gradient clipping
                double threshold = 1.0;
                gradient = std::max(-threshold, std::min(gradient, threshold));

                // Mise à jour des poids
                _weights[i]->coeffRef(r, c) += gradient;
            }
        }
    }
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
    std::cout << "training on dataSet of size :" << input.size() << std::endl;
    for (int i = 0 ; i < input.size() ; i++) {
        std::cout << "board " << i << std::endl;
        propagateForward(input[i]);
        backPropagation(output[i]);
    }
}

void NeuralNetwork::saveToFile(std::string filename)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << _config.size() << std::endl;
        for (int i = 0 ; i < _config.size() ; i++) {
            file << _config[i] << " ";
        }
        file << std::endl;

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
        int configSize;
        file >> configSize;
        _config.clear();
        _neurons.clear();
        _weights.clear();
        _bias.clear();
        _costs.clear();
        _caches.clear();
        for (int i = 0 ; i < configSize ; i++) {
            int tmp;
            file >> tmp;
            _neurons.push_back(new Vector(tmp));
            _costs.push_back(new Vector(tmp));
            _caches.push_back(new Vector(tmp));
            _config.push_back(tmp);
            for (int j = 0 ; j < _config.size() ; j++) {
                std::cout << _config[j] << " ";
            }
            std::cout << std::endl;
        }

        for (int i = 0 ; i < _config.size() - 1 ; i++) {
            _weights.push_back(new Matrix(_config[i], _config[i + 1]));
            for (int j = 0 ; j < _weights[i]->rows() ; j++) {
                for (int k = 0 ; k < _weights[i]->row(j).size() ; k++) {
                    file >> _weights[i]->row(j)[k];
                }
            }
        }

        for (int i = 0 ; i < _config.size() - 1 ; i++) {
            _bias.push_back(new Vector(_config[i + 1]));
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