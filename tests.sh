#!/bin/bash

mkdir -p out

make clean
make

./test_LSB1_encrypted.sh
./test_LSB4_encrypted.sh
./test_LSBI_encrypted.sh