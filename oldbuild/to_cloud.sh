#!/bin/bash

set -e

source emsdk_linux.src
./wasm.sh
rsync -av --progress out-wasm/* earthfa.de:/var/www/html/eso
