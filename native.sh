mkdir -p ./out
rm -rf ./out/*
g++ ./src/main.cpp ./build/s7.o -o ./out/EsoBinaria \
  -std=c++17 \
  -DAPP="\"$1\"" \
  -lm -lstdc++ -DBUILD_TIME=$(date +%s) \
  -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image \
  -g

rm -rf ./out/assets
cp -r game/assets ./out/assets

exit 0

mkdir -p ./out/EsoBinaria_Install
rm -rf ./out/*.DS_Store
rm -rf ./out/*.dSYM
mv ./out/* ./out/EsoBinaria_Install
mkdir -p ./out/EsoBinaria_Bundle
# Create a self-extracting archive
# This script uses the makeself utility to create a self-extracting archive.
# Parameters:
# ./out - The directory to be archived.
# ./out/EsoBinaria.run - The output file for the self-extracting archive.
# "EsoBinaria Bundle" - The label or description for the archive.
# ./imp - The command to run after extraction.
makeself ./out/EsoBinaria_Install ./out/EsoBinaria_Bundle/EsoBinaria "EsoBinaria Bundle" ./EsoBinaria

# Zip the bundle and install dirs
cd ./out
zip -r ./EsoBinaria_Bundle.zip ./EsoBinaria_Bundle
zip -r ./EsoBinaria_Install.zip ./EsoBinaria_Install

