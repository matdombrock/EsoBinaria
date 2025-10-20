FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \
    cmake \
    build-essential \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev
