#!/bin/bash

set -e

if [ ! -f ./build/s7.o ]; then
  echo "Compiling s7 for native..."
  gcc -c ./lib/s7/s7.c -o ./build/s7.o -std=c11
else
  echo "s7.o already exists, skipping compilation."
fi
