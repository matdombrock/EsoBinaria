#rm -rf ./out/*
mkdir -p ./out
g++ ./src/main.cpp ./build/s7.o -o ./out/imp.app \
 -std=c++17 \
 -DAPP="\"$1\"" \
 -lm -lstdc++\
 -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image \
 -g
rm -rf ./out/assets
cp -r game/assets ./out/assets