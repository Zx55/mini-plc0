#!/bin/bash

cd /home

if [ ! -d miniplc0-compiler ]; then
    git clone https://github.com/BUAA-SE-Compiling/miniplc0-compiler.git

    cd miniplc0-compiler
    git submodule init
    git submodule update
fi

cd /home
rm -rf homework
cp -rf miniplc0-compiler homework

cp mini-plc0/tokenizer/tokenizer.cpp homework/tokenizer/tokenizer.cpp
cp mini-plc0/analyser/analyser.cpp homework/analyser/analyser.cpp

apt install -y zip

zip -r /home/mini-plc0/homework.zip homework/*
