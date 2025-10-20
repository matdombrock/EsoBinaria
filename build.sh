#!/bin/bash

set -e

mkdir -p build

# Build s7
mkdir -p ./build
echo "Compiling s7 for native..."
g++ -c ./lib/s7/s7.c -o ./build/s7.o -std=c11

# Build the project
cd build
cmake ..
make
