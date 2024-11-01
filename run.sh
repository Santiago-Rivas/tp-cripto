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

# ./bin/stegobmp\
#     -embed\
#     -in "test/image.jpg"\
#     -p "img/samples/ejemplo.bmp" \
#     -out "out/out_LSB1.bmp"\
#     -pass "hola"\
#     -steg "LSB1"

# ./bin/stegobmp\
#     -extract\
#     -p "out/out_LSB1.bmp"\
#     -out "out/f1_LSB1"\
#     -pass "hola"\
#     -steg "LSB1"


# ./bin/stegobmp\
#     -embed\
#     -in "test/image.jpg"\
#     -p "img/samples/ejemplo.bmp" \
#     -out "out/out_LSB4.bmp"\
#     -pass "sorpresa"\
#     -a "aes128"\
#     -m "ofb"\
#     -steg "LSB4"

# ./bin/stegobmp\
#     -extract\
#     -p "out/out_LSB4.bmp"\
#     -out "out/f1_LSB4"\
#     -pass "sorpresa"\
#     -a "aes128"\
#     -m "ofb"\
#     -steg "LSB4"


# ./bin/stegobmp\
#     -embed\
#     -in "test/image.jpg"\
#     -p "img/samples/ejemplo.bmp" \
#     -out "out/out_LSBI.bmp"\
#     -pass "hola"\
#     -steg "LSBI"

# ./bin/stegobmp\
#     -extract\
#     -p "out/out_LSBI.bmp"\
#     -out "out/f1_LSBI"\
#     -pass "hola"\
#     -steg "LSBI"



# ./bin/stegobmp\
#     -embed\
#     -in "test/video.wmv"\
#     -p "img/samples/sample.bmp" \
#     -out "out/sample_LSB4.bmp"\
#     -steg "LSB4"\
#     -pass "sorpresa"\
#     -a "aes128"\
#     -m "ofb"\

./bin/stegobmp\
    -extract\
    -p "out/OUTPUT.bmp"\
    -out "out/OUTPUT_NUESTRO.wmv"\
    -steg "LSB4"\
    -pass "sorpresa"\
    -a "aes128"\
    -m "ofb"\


# ./bin/stegobmp -extract -p "imagenmas1.bmp" -out "mensaje1" -steg LSBI -a 3des -m cbc -pass "oculto"

# echo TEST 2

# algos=("aes128" "aes192" "aes256" "3des")
# modes=("ecb" "cfb" "ofb" "cbc")
# steg_modes=("LSBI")


# # Loop through all combinations of algos and modes
# for steg in "${steg_modes[@]}"; do
#     for algo in "${algos[@]}"; do
#         for mode in "${modes[@]}"; do
#             echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

#             EMBED_IN="test/test_1.txt"
#             EMBED_OUT="juani_bmp/imagenmas1_${steg}_${algo}_${mode}.bmp"
#             EXTRACT_OUT="juani_bmp/imagenmas1_${steg}_${algo}_${mode}_OUT"

#             # ./bin/stegobmp \
#             #     -embed \
#             #     -in $EMBED_IN \
#             #     -p "img/ejemplo2024sinEnc/lado.bmp" \
#             #     -out $EMBED_OUT \
#             #     -pass "hola" \
#             #     -steg "$steg" \
#             #     -a "$algo" \
#             #     -m "$mode"

#             ./bin/stegobmp \
#                 -extract \
#                 -p $EMBED_OUT \
#                 -out $EXTRACT_OUT \
#                 -pass "hola" \
#                 -steg "$steg" \
#                 -a "$algo" \
#                 -m "$mode"


#             # if [ $? -ne 0 ]; then
#             #     echo "RUN ERROR."
#             #     exit
#             # fi

#             # diff $EMBED_IN $EXTRACT_OUT.txt


#             # if [ $? -ne 0 ]; then
#             #     echo "OUTPUT IS DIFFERENT!"
#             #     exit
#             # fi
#         done
#     done
# done
