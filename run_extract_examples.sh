#!/bin/bash

OUTPUT="out_ejemplos/"
input_dir="img/ejemplo2024sinEnc"

rm -rf $OUTPUT
mkdir -p $OUTPUT

make clean
make

steg_modes=("LSB1" "LSB4" "LSBI")
algos=("aes128" "aes192" "aes256" "3des")
modes=("ecb" "cfb" "ofb" "cbc")

./bin/stegobmp\
    -extract\
    -p "img/ejemplo2024sinEnc/ladoLSB1.bmp"\
    -out "out_ejemplos/ladoLSB1"\
    -steg "LSB1"

./bin/stegobmp\
    -extract\
    -p "img/ejemplo2024sinEnc/ladoLSB4.bmp"\
    -out "out_ejemplos/ladoLSB4"\
    -steg "LSB4"

./bin/stegobmp\
    -extract\
    -p "img/ejemplo2024sinEnc/ladoLSBI.bmp"\
    -out "out_ejemplos/ladoLSBI"\
    -steg "LSBI"