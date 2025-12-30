#! /bin/sh
cmake -S . -B ./build &&
cp ./config.txt ./build &&
cd build && make
