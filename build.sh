#!/bin/bash

set -e

mkdir -p build

# Build s7
if [ ! -f ./build/s7.o ]; then
  echo "Compiling s7 for native..."
  gcc -c ./lib/s7/s7.c -o ./build/s7.o -std=c11
else
  echo "s7.o already exists, skipping compilation."
fi

# Build the project
cd build
cmake ..
make

# Copy the SDL libraries
sdl_dir="out/lib"
mkdir -p $sdl_dir

cp /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0 $sdl_dir/
cp /usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so.0 $sdl_dir/
cp /usr/lib/x86_64-linux-gnu/libSDL2_mixer-2.0.so.0 $sdl_dir/
cp /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0 $sdl_dir/
cp /usr/lib/x86_64-linux-gnu/libFLAC.so.12 $sdl_dir/

# Copy the launch script
cp ../build_res/launch.EsoBinaria.sh ./out/launch.EsoBinaria.sh
