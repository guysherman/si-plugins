#! /bin/bash

export COMPILER_PREFIX=x86_64-w64-mingw32-
export AR="$COMPILER_PREFIX"ar
export CC="$COMPILER_PREFIX"gcc
export CXX="$COMPILER_PREFIX"g++
export WIN32=true
export MINGW64=true

make clean
make
