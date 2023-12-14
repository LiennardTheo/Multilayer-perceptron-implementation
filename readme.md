# Funky Brain

This project is an implementation from scratch of a neural network for classification. The neural network is implemented in C++ and uses Eigen library for matrix computation. It provides a simple and efficient way to create, train and exploit a Neural network.

## Compilation

The project is compiled using cmake and you can use the following commands:

1. First you need to generate the build files using Cmake:

```
    cmake -S . -B build/
```

2. Once the build files are generated, execute the following command to build the project:

```
    cmake --build build
```

This will create a binary named myTorch.

## Usage

Launches the program by executing the binary and providing the arguments that correspond your needs:

```
    ./myTorch [-n <layers> || -l <load_file>] (-t (train) | -p (predict)) -s <save_file> -r <learning_rate> -m <activation_function> FILE
```

### Description

This program can be use to perform various operation on a neural network, here is a list of all the available options:

`-n <layers>`: create a neural network with the desired layers, for instance, a ```-n 64 32 16 16 4``` creates a neural network with 64 input neurons and 4 output neurons.

`-l <loadfile>`: loads a neural network located in `loadfile`, it loads the weights, the biases, the learning rate and the activation function

`-s <savefile>`: save a neural network in a file named `savefile`, create the file if it doesn't exist or overwrite it. 

`-t`: trains the network from a dataset

`-p`: checks the performance of the network from a dataset

`-r <learning_rate>`: an hyperparameter that define how strongly a single data modifies the weigths of the neural network (default 0.005)

`-m <activation_function>`: change the methods that calculates the output of a neuron ("sigmoid" and "tanh" are currently handled)

## Benchmark

