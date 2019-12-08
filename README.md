# mini-plc0

A mini plc0 compiler written in C++17.

See [handbook](https://github.com/BUAA-SE-Compiling/miniplc0-handbook/blob/master/Readme.md) and [vm-standard](https://github.com/BUAA-SE-Compiling/miniplc0-vm-standards/blob/master/Readme.md).

## Usage

First clone this repo in `/home/mini-plc0`.

* Write plc0 source code in `toolchain/in.plc0` and run [build.sh](./build.sh)

* Write test case in [`test_tokenizer.cpp`](./tests/test_tokenizer.cpp), [`test_analyser.cpp`](./tests/test_analyser.cpp) and run `toolchain/miniplc0_test` after build.