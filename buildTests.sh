#!/bin/sh
clang++ -o ./binary/testmain ./tests/testmain.cpp ./source/address.cpp ./source/geometry.cpp ./source/image.cpp ./source/partition.cpp ./source/sector.cpp
clang++ -o ./binary/testimg ./tests/testimg.cpp ./source/address.cpp ./source/geometry.cpp ./source/image.cpp ./source/partition.cpp ./source/sector.cpp
