#!/bin/bash
opt_arg="-O3"
if [ "$1" == "--debug" -o "$1" == "-d" ]; then
    opt_arg="-O1"
fi
echo em++ -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 $opt_arg -Wall -std=c++11 -o bin/basic.html -I. basic.parser.cpp code_writer_c.cpp code_writer_dump.cpp main.cpp ast/*.cpp
em++ -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 $opt_arg -Wall -std=c++11 -o bin/basic.html -I. basic.parser.cpp code_writer_c.cpp code_writer_dump.cpp main.cpp ast/*.cpp
