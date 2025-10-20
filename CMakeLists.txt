cmake_minimum_required(VERSION 3.10)
project(EsoBinaria CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get current UNIX timestamp for build time
string(TIMESTAMP BUILD_TIME "%s" UTC)

# Add your main source and prebuilt object
add_executable(EsoBinaria src/main.cpp build/s7.o)

# Define compile-time definitions
add_definitions(
  -DAPP="\"esobinaria\""
  -DBUILD_TIME=${BUILD_TIME}
)

# Link libraries
target_link_libraries(EsoBinaria
  m
  stdc++
  SDL2 SDL2_ttf SDL2_mixer SDL2_image
)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Copy assets
add_custom_command(
  TARGET EsoBinaria POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/game/assets ${CMAKE_BINARY_DIR}/out/assets
)

# Move output binary
add_custom_command(
  TARGET EsoBinaria POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E rename
    $<TARGET_FILE:EsoBinaria> ${CMAKE_BINARY_DIR}/out/EsoBinaria
)
