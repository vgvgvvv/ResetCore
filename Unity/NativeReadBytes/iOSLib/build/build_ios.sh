#!/usr/bin/env bash

mkdir build_ios
cd build_ios

cmake \
    -DIOS=true \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -GXcode \
    ../..

