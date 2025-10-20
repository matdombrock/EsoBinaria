#!/bin/bash

set -e

mkdir -p build

# Build s7
mkdir -p ./build
echo "Compiling s7 for wasm..."
emcc -c ./lib/s7/s7.c -o ./build/s7.wasm.o -std=c11

# Build the project
cd build
emcmake cmake -DESO_WASM=ON ..
make
