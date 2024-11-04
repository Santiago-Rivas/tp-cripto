#!/bin/bash

OUTPUT_LSB1="out_test_LSB1/lsb1"
input_dir="img/files_to_embed"

rm -rf $OUTPUT_LSB1
mkdir -p $OUTPUT_LSB1

make clean
make

algos=("aes128" "aes192" "aes256" "3des")
modes=("ecb" "cfb" "ofb" "cbc")
steg_modes=("LSB1")


# Loop through all combinations of algos and modes
for steg in "${steg_modes[@]}"; do
    for algo in "${algos[@]}"; do
        for mode in "${modes[@]}"; do
            echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

            EMBEDDED_DATA="img/samples/sample.bmp"
            EMBED_IN="$input_dir/text.txt"
            EMBED_OUT="$OUTPUT_LSB1/TEXT_${steg}_${algo}_${mode}.bmp"
            EXTRACT_OUT="$OUTPUT_LSB1/TEXT_${steg}_${algo}_${mode}"

            ./bin/stegobmp \
                -embed \
                -in $EMBED_IN \
                -p $EMBEDDED_DATA \
                -out $EMBED_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"

            ./bin/stegobmp \
                -extract \
                -p $EMBED_OUT \
                -out $EXTRACT_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"
        done
    done
done

# Loop through all combinations of algos and modes
for steg in "${steg_modes[@]}"; do
    for algo in "${algos[@]}"; do
        for mode in "${modes[@]}"; do
            echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

            EMBEDDED_DATA="img/samples/sample.bmp"
            EMBED_IN="$input_dir/image.jpg"
            EMBED_OUT="$OUTPUT_LSB1/IMAGE_${steg}_${algo}_${mode}.bmp"
            EXTRACT_OUT="$OUTPUT_LSB1/IMAGE_${steg}_${algo}_${mode}"

            ./bin/stegobmp \
                -embed \
                -in $EMBED_IN \
                -p $EMBEDDED_DATA \
                -out $EMBED_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"

            ./bin/stegobmp \
                -extract \
                -p $EMBED_OUT \
                -out $EXTRACT_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"
        done
    done
done


# Loop through all combinations of algos and modes
for steg in "${steg_modes[@]}"; do
    for algo in "${algos[@]}"; do
        for mode in "${modes[@]}"; do
            echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

            EMBEDDED_DATA="img/samples/sample.bmp"
            EMBED_IN="$input_dir/video.wmv"
            EMBED_OUT="$OUTPUT_LSB1/VIDEO_${steg}_${algo}_${mode}.bmp"
            EXTRACT_OUT="$OUTPUT_LSB1/VIDEO_${steg}_${algo}_${mode}"

            ./bin/stegobmp \
                -embed \
                -in $EMBED_IN \
                -p $EMBEDDED_DATA \
                -out $EMBED_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"

            ./bin/stegobmp \
                -extract \
                -p $EMBED_OUT \
                -out $EXTRACT_OUT \
                -pass "hola" \
                -steg "$steg" \
                -a "$algo" \
                -m "$mode"
        done
    done
done