# brew install sdl2 sdl2_ttf sdl2_mixer sdl2_image glew

rm -rf ./out/*
g++ -g ./src/main.cpp ./build/s7.o -o ./out/imp.app \
 -std=c++11 \
 -DAPP="\"$1\"" \
 -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image \
 -I/opt/homebrew/include -L/opt/homebrew/lib -lGLEW -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
cp -r game/assets ./out/assets