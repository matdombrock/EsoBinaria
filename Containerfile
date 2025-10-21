FROM ubuntu:24.04

# Core deps
RUN apt-get update && \
    apt-get install -y \
    cmake \
    build-essential \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev 

# Emscripten deps
RUN apt-get install -y \
    git \
    python3 \
    python3-pip \
    nodejs 

# Setup Emscripten
WORKDIR /emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git .
RUN ./emsdk install latest
RUN ./emsdk activate latest
# Load the Emscripten environment variables
RUN echo "source /emsdk/emsdk_env.sh" >> ~/.bashrc
