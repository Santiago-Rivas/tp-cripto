#!/bin/bash

mkdir -p out

make clean
make

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit
fi


echo "Compilation successful."

echo TEST 1

./bin/stegobmp\
    -embed\
    -in "test/test_1.txt"\
    -p "img/ejemplo2024sinEnc/lado.bmp" \
    -out "out/out.bmp"\
    -pass "hola"\
    -steg "LSB1"

./bin/stegobmp\
    -extract\
    -p "out/out.bmp"\
    -out "out/hola"\
    -pass "hola"\
    -steg "LSB1"

# ./bin/stegobmp\
#     -extract\
#     -p "out/out.bmp" \
#     -out "out/mensaje_ext.txt"\
#     -steg "LSB1"
# 

# ./bin/stegobmp \
#     -extract \
#     -p "out/juani_y_pato.bmp"  \
#     -out "out/extracted" \
#     -pass "hola" \
#     -steg "LSB1" \
#     -a "aes128" \
#     -m "cbc"



# ./bin/stegobmp -extract -p "imagenmas1.bmp" -out "mensaje1" -steg LSBI -a 3des -m cbc -pass "oculto"

echo TEST 2

algos=("aes128" "aes192" "aes256" "3des")
modes=("ecb" "cfb" "ofb" "cbc")

# Loop through all combinations of algos and modes
for algo in "${algos[@]}"; do
    for mode in "${modes[@]}"; do
        echo "TESTING: Algorithm = $algo, Mode = $mode"

        EMBED_IN="test/test_1.txt"
        EMBED_OUT="out/imagenmas1_${algo}_${mode}.bmp"
        EXTRACT_OUT="out/imagenmas1_${algo}_${mode}_OUT"

        ./bin/stegobmp \
            -embed \
            -in $EMBED_IN \
            -p "img/ejemplo2024sinEnc/lado.bmp" \
            -out $EMBED_OUT \
            -pass "hola" \
            -steg "LSB1" \
            -a "$algo" \
            -m "$mode"

        ./bin/stegobmp \
            -extract \
            -p $EMBED_OUT \
            -out $EXTRACT_OUT \
            -pass "hola" \
            -steg "LSB1" \
            -a "$algo" \
            -m "$mode"


        if [ $? -ne 0 ]; then
            echo "RUN ERROR."
            exit
        fi

        diff $EMBED_IN $EXTRACT_OUT.txt


        if [ $? -ne 0 ]; then
            echo "OUTPUT IS DIFFERENT!"
            exit
        fi
    done
done
