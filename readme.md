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

`-n <layers>`: create a neural network with the desired layers, for instance, a ```-n 64 32 16 16 4``` creates a neural network with 64 input neurons and 4 output neurons, 32, 16 and 16 are hidden layers.

`-l <loadfile>`: loads a neural network located in `loadfile`, it loads the architecture, the weights, the biases, the learning rate and the activation function

`-s <savefile>`: save a neural network in a file named `savefile`, create the file if it doesn't exist or overwrite it. 

`-t <dataSize>`: trains the network from a dataset, `dataSize` is the number of exemples that will be parsed from the file, starting from the top.

`-p`: checks the performance of the network from a datasets, `dataSize` is the number of exemples that will be parsed from the file, starting from the top.

`-r <learning_rate>`: an hyperparameter that define how strongly a single data modifies the weigths of the neural network (default 0.005)

`-m <activation_function>`: change the methods that calculates the output of a neuron ("sigmoid" and "tanh" are currently handled)

## Benchmark

```mermaid
xychart-beta
    title "Sales Revenue"
    x-axis [jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec]
    y-axis "Revenue (in $)" 4000 --> 11000
    bar [5000, 6000, 7500, 8200, 9500, 10500, 11000, 10200, 9200, 8500, 7000, 6000]
    line [5000, 6000, 7500, 8200, 9500, 10500, 11000, 10200, 9200, 8500, 7000, 6000]
```