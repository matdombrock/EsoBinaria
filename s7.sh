#!/bin/bash
mkdir -p ./build
echo "Compiling for native..."
g++ -c ./lib/s7/s7.c -o ./build/s7.o -std=c11
echo "Compiling for wasm..."
emcc -c ./lib/s7/s7.c -o ./build/s7.wasm.o -std=c11