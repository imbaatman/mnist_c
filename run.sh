#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd "$SCRIPT_DIR"

if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found in the current directory."
    exit 1
fi

DATA_DIR="data"
REQUIRED_FILES=("train-images.idx3-ubyte" "train-labels.idx1-ubyte" "t10k-images.idx3-ubyte" "t10k-labels.idx1-ubyte")

for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$DATA_DIR/$file" ]; then
        echo "Error: Required data file $file not found in $DATA_DIR directory."
        exit 1
    fi
done

echo "Cleaning previous build..."
make clean

# Compile the project
echo "Compiling the project..."
make

echo "Running the MNIST classifier..."
make run

echo "Execution completed."
