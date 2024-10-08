rm -rf ./out/*
g++ ./src/main.cpp ./build/s7.o -o ./out/imp.app \
 -std=c++11 \
 -DAPP="\"$1\"" \
 -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
cp -r game/assets ./out/assets