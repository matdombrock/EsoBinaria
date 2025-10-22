#!/bin/bash

set -e

mkdir -p buildwasm

# Build s7
./tools/build.s7.wasm.sh

# Build the project
cd buildwasm
echo `which emcmake`
emcmake cmake -DESO_WASM=ON ..
make
