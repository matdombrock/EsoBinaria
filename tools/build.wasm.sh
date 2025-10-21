#!/bin/bash

set -e

mkdir -p buildwasm

# Build s7
if [ ! -f ./buildwasm/s7.wasm.o ]; then
  echo "Compiling s7 for wasm..."
  emcc -c ./lib/s7/s7.c -o ./buildwasm/s7.wasm.o -std=c11
else
  echo "s7.o already exists, skipping compilation."
fi

# Build the project
cd buildwasm
echo `which emcmake`
emcmake cmake -DESO_WASM=ON ..
make
