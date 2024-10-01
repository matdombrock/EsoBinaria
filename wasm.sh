#!/bin/bash
rm -rf ./out/*
emcc ./src/main.cpp -o out/index.html \
 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'\
 -DAPP="\"$1\"" \
 --preload-file examples/assets@/assets \
 --shell-file shell_minimal.html

 