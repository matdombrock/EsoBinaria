#!/bin/bash

set -e

mkdir -p buildwasm

# Build s7
echo "Compiling s7 for wasm..."
emcc -c ./lib/s7/s7.c -o ./buildwasm/s7.wasm.o -std=c11

# Build the project
cd buildwasm
emcmake cmake -DESO_WASM=ON ..
make
