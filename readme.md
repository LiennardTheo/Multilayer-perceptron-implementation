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

So first of all i tested my neural network implementation by detecting checkmate or not, it's a pretty easy task so it was perfect to test my algorithm, here is a diagram of how it went:

![Alt text](/images/1output.png "Optional title")

So as you can see, it started with a pretty bad accuracy, but after some tweaking of the hyperparameters, it reached a solid 95 % accuracy.

The best hyperparameters i found were:

- 64 input neurons
- 32 hidden neurons
- 16 hidden neurons
- 1 output neuron

- learning rate: 0.005
- activation function: tanh

and the best was also to train the network with a large dataset (around 500 000 exemples) and that's because a lot of the data is redundant, so it's better to have a lot of data to train the network.


Then i tried to train a network to detect 3 states of the game, Checkmates, Pats, and Nothing, so i had to change the architecture of the network, i tried a lot of different architectures, but the best i found was:

- 64 input neurons
- 26 hidden neurons
- 43 hidden neurons
- 3 output neurons

- learning rate: 0.009
- activation function: tanh

I found it by trying all possible combinations of neurons, and i found that the best was to have a lot of hidden neurons, and i also tried to have a lot of hidden layers, but it didn't improve the accuracy, so i kept it with 2 hidden layers.

Here is a diagram of how it went:

![Alt text](/images/2output.png "Optional title")

So as you can see, i made a comparaison between the sigmoid and the tanh activation function.
The sigmoid function have a better output on certain really precise cases, but the tanh function is more stable and have a better accuracy overall, so i kept it.

The tanh also recognize a lot better checkmates than the sigmoid, and that's because the sigmoid function is more sensitive to the vanishing gradient problem, so it's better to use the tanh function.

## Optimizations

I tried to optimize the code as much as possible, so i used Eigen library for matrix computation.

The main optimization i did was to use a gradient descent algorithm ,coupled with a clipping of this gradient, it is a way to avoid the vanishing gradient problem, and it also makes the training faster. The clipping is done by dividing the gradient by its norm, if the norm is greater than 1.

I also did a shuffle of the dataset before training, so that the network doesn't learn the data in the same order, and it also helps to avoid the vanishing gradient problem.


## Further Enhancements

Future enhancements could involve exploring additional activation functions, experimenting with different optimization algorithms, and potentially incorporating more sophisticated network architectures.