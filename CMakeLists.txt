cmake_minimum_required(VERSION 3.10)
project(Esobinaria CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ESO_WASM "Build for WebAssembly using Emscripten" OFF)

# Get current UNIX timestamp for build time
string(TIMESTAMP BUILD_TIME "%s" UTC)

if(ESO_WASM)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_COMPILER em++)
  set(CMAKE_C_COMPILER emcc)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s ALLOW_MEMORY_GROWTH -s \"EXPORTED_RUNTIME_METHODS=['FS']\" -s FORCE_FILESYSTEM=1 --shell-file ${CMAKE_SOURCE_DIR}/wasm_res/shell_minimal.html --preload-file ${CMAKE_SOURCE_DIR}/game/assets@/assets")
  add_executable(Esobinaria src/main.cpp build/s7.wasm.o)
else()
  add_executable(Esobinaria src/main.cpp build/s7.o)
endif()

add_definitions(
  -DAPP="esobinaria"
  -DBUILD_TIME=${BUILD_TIME}
)

if(NOT ESO_WASM)
  target_link_libraries(Esobinaria
    m
    stdc++
    SDL2 SDL2_ttf SDL2_mixer SDL2_image
  )
endif()

set(CMAKE_BUILD_TYPE Debug)

# Post-build: Native
if(NOT ESO_WASM)
  add_custom_command(
    TARGET Esobinaria POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:Esobinaria> ${CMAKE_BINARY_DIR}/out/Esobinaria
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/game/assets ${CMAKE_BINARY_DIR}/out/assets
  )
endif()

# Post-build: WASM
if(ESO_WASM)
  add_custom_command(
    TARGET Esobinaria POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/outhtml
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/Esobinaria.html ${CMAKE_BINARY_DIR}/outhtml/index.html
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/Esobinaria.js ${CMAKE_BINARY_DIR}/outhtml/Esobinaria.js
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/Esobinaria.wasm ${CMAKE_BINARY_DIR}/outhtml/Esobinaria.wasm
    COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/Esobinaria.data ${CMAKE_BINARY_DIR}/outhtml/Esobinaria.data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/game/assets ${CMAKE_BINARY_DIR}/outhtml/assets
  )
endif()
