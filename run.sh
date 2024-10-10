#!/bin/bash

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
    -out "out.bmp"\
    -pass "hola"\
    -steg "LSBI"

# echo TEST 2
# ./bin/stegobmp -extract -p "imagenmas1.bmp" -out "mensaje1" -steg LSBI -a 3des -m cbc -pass "oculto"

algos=("AES128" "AES192" "AES256" "DES3")
modes=("ECB" "CFB" "OFB" "CBC")

# Loop through all combinations of algos and modes
for algo in "${algos[@]}"; do
    for mode in "${modes[@]}"; do
        echo "TESTING: Algorithm = $algo, Mode = $mode"
        ./bin/stegobmp \
            -embed \
            -in "test/test_1.txt" \
            -p "img/ejemplo2024sinEnc/lado.bmp" \
            -out "imagenmas1_${algo}_${mode}.bmp" \
            -pass "hola" \
            -steg "LSBI" \
            -a "$algo" \
            -m "$mode"

        if [ $? -ne 0 ]; then
            echo "RUN ERROR."
            exit
        fi
    done
done
