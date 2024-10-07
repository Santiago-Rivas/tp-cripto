#!/bin/bash

make clean
make

if [ $? -eq 0 ]; then
    echo "Compilation successful."

    echo TEST 1
    ./bin/stegobmp -embed -in "mensaje1.txt" -p "imagen1.bmp" -out "imagenmas1.bmp" -steg LSBI

    echo TEST 2
    ./bin/stegobmp -extract -p "imagenmas1.bmp" -out "mensaje1" -steg LSBI -a 3des -m cbc -pass "oculto"
else
    echo "Compilation failed."
fi


