#!/bin/bash

# Launch the game using the bundled libraries
# To launch using native libraies just run ./EsoBinaria
# libSDL2
# libSDL2_image
# libSDL2_mixer
# libSDL2_ttf
# libFLAC

LD_LIBRARY_PATH="$PWD/lib:$LD_LIBRARY_PATH" ./EsoBinaria
