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


# # Loop through all combinations of algos and modes
# for steg in "${steg_modes[@]}"; do
#     echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

#     for file in "$input_dir"/*; do
#         if [ ! -f "$file" ]; then
#             echo "No files found in $input_dir directory."
#             break
#         fi
#         echo "Testing $file"

#         filename=$(basename "$file")
#         EXTRACT_OUT="${OUTPUT}/${filename}_${steg}_${algo}_${mode}"

#         ./bin/stegobmp \
#             -extract \
#             -p "$file" \
#             -out "$EXTRACT_OUT" \
#             -steg "$steg" \

#     done
# done

# # Loop through all combinations of algos and modes
# for steg in "${steg_modes[@]}"; do
#     for algo in "${algos[@]}"; do
#         for mode in "${modes[@]}"; do
#             echo ""
#             echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

#             for file in "$input_dir"/*; do
#                 if [ ! -f "$file" ]; then
#                     echo "No files found in $input_dir directory."
#                     break
#                 fi
#                 echo "Testing $file"

#                 filename=$(basename "$file")
#                 EXTRACT_OUT="${OUTPUT}/${filename}_${steg}"

#                     ./bin/stegobmp \
#                     -extract \
#                     -p "$file" \
#                     -out "$EXTRACT_OUT" \
#                     -steg "$steg" \
#                     -pass "sorpresa" \
#                     -a "$algo" \
#                     -m "$mode"

#                 # if [ $? -ne 0 ]; then
#                 #     echo "EXTRACT ERROR for file $file."
#                 #     exit
#                 # fi

#             done
#         done
#     done
# done
