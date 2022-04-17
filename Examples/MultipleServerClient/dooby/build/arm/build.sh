#!/bin/bash
export CC=/mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
export CXX=/mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
if [ -d "./build" ]; then
    cd build
    make
else
    mkdir build
    cd build
    cmake ../
    make
    cd ..
fi