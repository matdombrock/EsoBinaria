#!/bin/bash

set -e

mkdir -p build

# Build s7
./tools/build.s7.sh

# Build the project
cd build
# cmake ..
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make

# Packaging step
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

# Copy the compile commands
cp compile_commands.json ../compile_commands.json
