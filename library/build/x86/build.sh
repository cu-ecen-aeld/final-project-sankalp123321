#!/bin/bash
export CC=/usr/bin/cc
export CXX=/usr/bin/c++

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
