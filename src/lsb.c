#include "lsb.h"

#include <stdio.h>
#include <string.h>

size_t lsb1_embed(const uint8_t* data, size_t size, uint8_t* carrier, size_t carrier_size) {
    if (size * 8 > carrier_size) {
        return 0;
    }

    size_t embed_size = 0;

    for (size_t byte_i = 0; byte_i < size; byte_i++) {
        for (size_t bit_i = 0; bit_i < 8; bit_i++) {
            size_t embed_i = byte_i * 8 + bit_i;

            uint8_t data_bit = (data[byte_i] >> bit_i) & 0x01;

            carrier[embed_i] = (carrier[embed_i] & 0xFE) | data_bit;
            embed_size++;
        }
    }

    return embed_size;
}

size_t lsb1_extract(uint8_t* data_out, size_t size, const uint8_t* carrier, size_t carrier_size) {
    if (size * 8 > carrier_size) {
        return 0;
    }

    size_t embed_size = 0;
    
    for (size_t byte_i = 0; byte_i < size || size == 0; byte_i++) {
        data_out[byte_i] = 0;

        for (size_t bit_i = 0; bit_i < 8; bit_i++) {
            size_t embed_i = byte_i * 8 + bit_i;

            uint8_t data_bit = carrier[embed_i] & 0x01;

            data_out[byte_i] |= data_bit << bit_i;
            embed_size++;
        }

        if (size == 0 && data_out[byte_i] == '\0') {
            break;
        }
    }

    return embed_size;
}