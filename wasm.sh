#!/bin/bash
rm -rf ./out/*
emcc ./src/main.cpp ./build/s7.wasm.o -o out/index.html \
 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'\
 -sALLOW_MEMORY_GROWTH \
 -std=c++17 \
 -s "EXPORTED_RUNTIME_METHODS=['FS']" -s "FORCE_FILESYSTEM=1"\
 -DAPP="\"$1\"" \
 --preload-file game/assets@/assets \
 --shell-file shell_minimal.html