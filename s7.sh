#!/bin/bash
mkdir -p ./build
gcc -c ./lib/s7/s7.c -o ./build/s7.o -std=c11