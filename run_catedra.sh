#!/bin/bash

OUTPUT="out_catedra_bruh/"
input_dir="img/grupo16/"
rm -rf $OUTPUT
mkdir -p $OUTPUT

make clean
make

steg_modes=("LSB1" "LSB4" "LSBI")


# Loop through all combinations of algos and modes
for steg in "${steg_modes[@]}"; do
    echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

    for file in "$input_dir"/*; do
        if [ ! -f "$file" ]; then
            echo "No files found in $input_dir directory."
            break
        fi
        echo "Testing $file"

        filename=$(basename "$file")
        EXTRACT_OUT="${OUTPUT}/${filename}_${steg}_${algo}_${mode}_OUT.txt"

        ./bin/stegobmp \
            -extract \
            -p "$file" \
            -out "$EXTRACT_OUT" \
            -steg "$steg" \

    done
done


algos=("aes128" "aes192" "aes256" "3des")
modes=("ecb" "cfb" "ofb" "cbc")

# algos=("3des")
# modes=("ofb")

steg_modes=("LSB1" "LSB4" "LSBI")

# Loop through all combinations of algos and modes
for steg in "${steg_modes[@]}"; do
    for algo in "${algos[@]}"; do
        for mode in "${modes[@]}"; do
            echo "TESTING: Steg = $steg, Algorithm = $algo, Mode = $mode"

            for file in "$input_dir"/*; do
                if [ ! -f "$file" ]; then
                    echo "No files found in $input_dir directory."
                    break
                fi
                echo "Testing $file"

                filename=$(basename "$file")
                EXTRACT_OUT="${OUTPUT}/${filename}_${steg}_${algo}_${mode}_OUT.txt"

                ./bin/stegobmp \
                    -extract \
                    -p "$file" \
                    -out "$EXTRACT_OUT" \
                    -pass "sorpresa" \
                    -steg "$steg" \
                    -a "$algo" \
                    -m "$mode"

                # if [ $? -ne 0 ]; then
                #     echo "EXTRACT ERROR for file $file."
                #     exit
                # fi

            done
        done
    done
done
