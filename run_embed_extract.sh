#!/bin/bash

OUTPUT_LSB1="out_embedded/lsb1"
OUTPUT_LSB4="out_embedded/lsb4"
OUTPUT_LSBI="out_embedded/lsbi"
input_dir="img/files_to_embed"

rm -rf $OUTPUT_LSB1
rm -rf $OUTPUT_LSB4
rm -rf $OUTPUT_LSBI

mkdir -p $OUTPUT_LSB1
mkdir -p $OUTPUT_LSB4
mkdir -p $OUTPUT_LSBI

make clean
make

./bin/stegobmp\
    -embed\
    -in "$input_dir/image.jpg"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSB1/ejemplo_image_LSB1.bmp"\
    -steg "LSB1"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB1/ejemplo_image_LSB1.bmp"\
    -out "$OUTPUT_LSB1/ejemplo_image_LSB1"\
    -steg "LSB1"

./bin/stegobmp\
    -embed\
    -in "$input_dir/text.txt"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSB1/ejemplo_text_LSB1.bmp"\
    -steg "LSB1"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB1/ejemplo_text_LSB1.bmp"\
    -out "$OUTPUT_LSB1/ejemplo_text_LSB1"\
    -steg "LSB1"

./bin/stegobmp\
    -embed\
    -in "$input_dir/video.wmv"\
    -p "img/samples/sample.bmp" \
    -out "$OUTPUT_LSB1/ejemplo_video_LSB1.bmp"\
    -steg "LSB1"


./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB1/ejemplo_video_LSB1.bmp"\
    -out "$OUTPUT_LSB1/ejemplo_video_LSB1"\
    -steg "LSB1"

./bin/stegobmp\
    -embed\
    -in "$input_dir/image.jpg"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSB4/ejemplo_image_LSB4.bmp"\
    -steg "LSB4"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB4/ejemplo_image_LSB4.bmp"\
    -out "$OUTPUT_LSB4/ejemplo_image_LSB4"\
    -steg "LSB4"

./bin/stegobmp\
    -embed\
    -in "$input_dir/text.txt"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSB4/ejemplo_text_LSB4.bmp"\
    -steg "LSB4"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB4/ejemplo_text_LSB4.bmp"\
    -out "$OUTPUT_LSB4/ejemplo_text_LSB4"\
    -steg "LSB4"


./bin/stegobmp\
    -embed\
    -in "$input_dir/video.wmv"\
    -p "img/samples/sample.bmp" \
    -out "$OUTPUT_LSB4/ejemplo_video_LSB4.bmp"\
    -steg "LSB4"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSB4/ejemplo_video_LSB4.bmp"\
    -out "$OUTPUT_LSB4/ejemplo_video_LSB4"\
    -steg "LSB4"

./bin/stegobmp\
    -embed\
    -in "$input_dir/image.jpg"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSBI/ejemplo_image_LSBI.bmp"\
    -steg "LSBI"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSBI/ejemplo_image_LSBI.bmp"\
    -out "$OUTPUT_LSBI/ejemplo_image_LSBI"\
    -steg "LSBI"

./bin/stegobmp\
    -embed\
    -in "$input_dir/text.txt"\
    -p "img/samples/ejemplo.bmp" \
    -out "$OUTPUT_LSBI/ejemplo_text_LSBI.bmp"\
    -steg "LSBI"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSBI/ejemplo_text_LSBI.bmp"\
    -out "$OUTPUT_LSBI/ejemplo_text_LSBI"\
    -steg "LSBI"

./bin/stegobmp\
    -embed\
    -in "$input_dir/video.wmv"\
    -p "img/samples/sample.bmp" \
    -out "$OUTPUT_LSBI/ejemplo_video_LSBI.bmp"\
    -steg "LSBI"

./bin/stegobmp\
    -extract\
    -p "$OUTPUT_LSBI/ejemplo_video_LSBI.bmp"\
    -out "$OUTPUT_LSBI/ejemplo_video_LSBI"\
    -steg "LSBI"