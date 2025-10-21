cmake_minimum_required(VERSION 3.10)
project(EsoBinaria CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ESO_WASM "Build for WebAssembly using Emscripten" OFF)

# Get current UNIX timestamp for build time
string(TIMESTAMP BUILD_TIME "%s" UTC)

if(ESO_WASM)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_COMPILER em++)
  set(CMAKE_C_COMPILER emcc)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s ALLOW_MEMORY_GROWTH -s FORCE_FILESYSTEM=1")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s ALLOW_MEMORY_GROWTH -s \"EXPORTED_RUNTIME_METHODS=['FS']\" -s FORCE_FILESYSTEM=1 --shell-file ${CMAKE_SOURCE_DIR}/build_res/shell_minimal.html --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")
  add_executable(EsoBinaria src/main.cpp buildwasm/s7.wasm.o)
else()
  add_executable(EsoBinaria src/main.cpp build/s7.o)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_definitions(
  -DAPP="esobinaria"
  -DBUILD_TIME=${BUILD_TIME}
)

if(NOT ESO_WASM)
  target_link_libraries(EsoBinaria
    m
    stdc++
    SDL2 SDL2_ttf SDL2_mixer SDL2_image
  )
endif()

# set(CMAKE_BUILD_TYPE Debug)

# Post-build: Native
if(NOT ESO_WASM)
  add_custom_command(
    TARGET EsoBinaria POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/out
    COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:EsoBinaria> ${CMAKE_BINARY_DIR}/out/EsoBinaria
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/out/assets
  )
endif()

# Post-build: WASM
if(ESO_WASM)
  add_custom_command(
    TARGET EsoBinaria POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/out
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/EsoBinaria.html ${CMAKE_BINARY_DIR}/out/index.html
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/EsoBinaria.js ${CMAKE_BINARY_DIR}/out/EsoBinaria.js
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/EsoBinaria.wasm ${CMAKE_BINARY_DIR}/out/EsoBinaria.wasm
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/EsoBinaria.data ${CMAKE_BINARY_DIR}/out/EsoBinaria.data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/out/assets
  )
endif()
