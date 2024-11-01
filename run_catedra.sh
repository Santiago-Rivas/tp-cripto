#!/bin/bash

OUTPUT="out_catedra/"
input_dir="img/grupo16"

# OUTPUT="out_ejemplos/"
# input_dir="img/ejemplo2024sinEnc"

rm -rf $OUTPUT
mkdir -p $OUTPUT

make clean
make

steg_modes=("LSB1" "LSB4" "LSBI")

algos=("aes128" "aes192" "aes256" "3des")
modes=("ecb" "cfb" "ofb" "cbc")

./bin/stegobmp\
    -extract\
    -p "img/grupo16/kings.bmp"\
    -out "out_catedra/kings_LSB1"\
    -steg "LSB1"

./bin/stegobmp\
    -extract\
    -p "img/grupo16/lima.bmp"\
    -out "out_catedra/lima_LSB4"\
    -steg "LSB4"\
    -pass "sorpresa"\
    -a "aes128"\
    -m "ofb"

./bin/stegobmp\
    -extract\
    -p "img/grupo16/paris.bmp"\
    -out "out_catedra/paris_LSBI"\
    -steg "LSBI"