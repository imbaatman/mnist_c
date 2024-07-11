# MNIST Classifier in C

This project implements a simple MNIST digit classifier in C. It trains on the MNIST dataset and can classify handwritten digits from 0 to 9.

## Project Structure

```
.
├── build
│   └── MNIST (executable will be here after compilation)
├── data
│   ├── t10k-images.idx3-ubyte
│   ├── t10k-labels.idx1-ubyte
│   ├── train-images.idx3-ubyte
│   └── train-labels.idx1-ubyte
├── src
│   └── mnist.c
├── Makefile
├── run.sh
└── README.md
```

## Prerequisites

- GCC compiler
- Make build system
- MNIST dataset files (already included in the `data` directory)

## Setup

1. Clone this repository.

2. Ensure you have the MNIST dataset files in the `data` directory. The required files are:
   - `train-images.idx3-ubyte`
   - `train-labels.idx1-ubyte`
   - `t10k-images.idx3-ubyte`
   - `t10k-labels.idx1-ubyte`

   If you don't have these files, you can download them from the [MNIST database](http://yann.lecun.com/exdb/mnist/).

3. Make sure you have GCC and Make installed on your system. You can check this by running:
   ```
   gcc --version
   make --version
   ```

## Compilation

You can compile the project using the provided Makefile:

```
make
```

This will create the `MNIST` executable in the `build` directory.

## Running the Classifier

To run the MNIST classifier, you can use the provided shell script:

```
./run.sh
```

This script will:
1. Clean any previous build
2. Compile the project
3. Run the MNIST classifier

run these steps manually:

```
make clean
make
./build/MNIST
```

## How It Works

1. The program loads the MNIST training and test data from the `data` directory.
2. It trains a logistic regression model on the training data.
3. The model is then tested on the test data.
4. The program displays some sample classifications and the overall accuracy.

## Modifying the Code

The main source code is located in `src/mnist.c`. You can modify this file to experiment with different model architectures or hyperparameters.

Key hyperparameters you might want to adjust include:
- `LEARNING_RATE`: The step size for gradient descent
- `NUM_EPOCHS`: The number of training epochs
- `BATCH_SIZE`: The size of mini-batches for training
- `LAMBDA`: The L2 regularization parameter

After making changes, simply run `./run.sh` again to recompile and run the updated code.

## Troubleshooting

If you encounter any issues:

1. Ensure all required MNIST data files are present in the `data` directory.
2. Check that you have GCC and Make installed and accessible from the command line.
3. If you get any compilation warnings or errors, they will be displayed in the console. Address these issues in the `src/mnist.c` file.
