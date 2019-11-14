#!/bin/bash

cd /home/mini-plc0/

if [ ! -d build ]; then
    mkdir build
    cd build
else
    cd build
fi

cmake ..
make -j 8
cp ./miniplc0 ../toolchain/miniplc0

echo "!!build complete!!"

cd /home/mini-plc0/toolchain

if [ -f in.plc0 ]; then
    echo "!!test in.plc0!!"

    ./miniplc0 -l in.plc0 > out
    ./linux-amd64 -A -i out -o out.epf
    ./linux-amd64 -R -i out.epf
fi
