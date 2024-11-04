#!/bin/bash

OUTPUT="out_catedra/"
input_dir="img/grupo16"

rm -rf $OUTPUT
mkdir -p $OUTPUT

make clean
make

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

cp out_catedra/kings_LSB1.png out_catedra/kings_LSB1.zip
unzip out_catedra/kings_LSB1.zip -d out_catedra/kings_LSB1_extracted
rm out_catedra/kings_LSB1.zip

xxd -s 0x802cb6 -l 0x33 img/grupo16/loimposible.bmp > out_catedra/loimposible_hex.txt