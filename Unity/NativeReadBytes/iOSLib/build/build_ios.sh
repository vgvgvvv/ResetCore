#!/usr/bin/env bash

mkdir build_ios
cd build_ios

cmake \
    -DIOS=true \
    DIOS_PLATFORM=OS \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/ios.toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    ../..

