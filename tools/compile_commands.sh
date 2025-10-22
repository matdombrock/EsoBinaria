#!/bin/bash

set -e

mkdir -p build

./tools/build.s7.sh

cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cp compile_commands.json ..
