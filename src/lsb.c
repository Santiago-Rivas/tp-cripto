#include "lsb.h"

#include <stdio.h>

size_t lsb1_embed(uint8_t *data, size_t size, uint8_t *carrier,
                  size_t carrier_size) {
  if (size * 8 > carrier_size) {
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size; byte_i++) {
    for (size_t bit_i = 0; bit_i < 8; bit_i++) {
      size_t embed_i = byte_i * 8 + bit_i;

      uint8_t data_bit = (data[byte_i] >> (7 - bit_i)) & 0x01;

      carrier[embed_i] = (carrier[embed_i] & 0xFE) | data_bit;
      embed_size++;
    }
  }

  return embed_size;
}

size_t lsb1_extract(uint8_t *data_out, size_t size, const uint8_t *carrier,
                    size_t carrier_size) {
  if (size * 8 > carrier_size) {
    printf("size = %zu carrier_size = %zu too big!\n", size, carrier_size);
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size || size == 0; byte_i++) {
    data_out[byte_i] = 0;

    for (size_t bit_i = 0; bit_i < 8; bit_i++) {
      size_t embed_i = byte_i * 8 + bit_i;

      uint8_t data_bit = carrier[embed_i] & 0x01;
      data_out[byte_i] <<= 1;
      data_out[byte_i] |= data_bit;
      embed_size++;
    }
    // printf("Hexadecimal (uppercase): %c %X\n", data_out[byte_i],
    // data_out[byte_i]);  // Output: FF printf("Hexadecimal (uppercase): %X\n",
    // data_out[byte_i]);  // Output: FF

    if (size == 0 && data_out[byte_i] == '\0') {
      break;
    }
  }

  return embed_size;
}

size_t lsb4_embed(uint8_t *data, size_t size, uint8_t *carrier,
                  size_t carrier_size) {
  if (size * 2 > carrier_size) {
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size; byte_i++) {
    size_t embed_i = byte_i * 2;
    uint8_t high_nibble = (data[byte_i] >> 4) & 0xF;
    carrier[embed_i] = (carrier[embed_i] & 0xF0) | high_nibble;
    embed_size++;

    embed_i++;
    uint8_t low_nibble = data[byte_i] & 0xF;
    carrier[embed_i] = (carrier[embed_i] & 0xF0) | low_nibble;
    embed_size++;
  }

  return embed_size;
}

size_t lsb4_extract(uint8_t *data_out, size_t size, const uint8_t *carrier,
                    size_t carrier_size) {
  if (size * 2 > carrier_size) {
    printf("size = %zu carrier_size = %zu too big!\n", size, carrier_size);
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size || size == 0; byte_i++) {
    if (byte_i * 2 >= carrier_size)
      break; // Safeguard for carrier size

    size_t embed_i = byte_i * 2;
    uint8_t high_nibble = carrier[embed_i] & 0xF;
    data_out[byte_i] = high_nibble << 4;

    embed_i++;
    uint8_t low_nibble = carrier[embed_i] & 0xF;
    data_out[byte_i] |= low_nibble;
    embed_size++;

    if (size == 0 && data_out[byte_i] == '\0') {
      break;
    }
  }

  return embed_size;
}

size_t lsbi_embed(uint8_t *data, size_t size, uint8_t *carrier,
                  size_t carrier_size) {
  if (size * 8 > carrier_size) {
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size; byte_i++) {
    for (size_t bit_i = 0; bit_i < 8; bit_i++) {
      size_t embed_i = byte_i * 8 + bit_i;

      uint8_t data_nibble = (data[byte_i] >> ((7 - bit_i) * 4)) & 0xF;

      carrier[embed_i] = (carrier[embed_i] & 0xF0) | data_nibble;
      embed_size++;
    }
  }

  return embed_size;
}

size_t lsbi_extract(uint8_t *data_out, size_t size, const uint8_t *carrier,
                    size_t carrier_size) {
  if (size * 8 > carrier_size) {
    printf("size = %zu carrier_size = %zu too big!\n", size, carrier_size);
    return 0;
  }

  uint8_t byte = 0;
  uint32_t hidden_iter = 0;
  uint32_t carrier_iter = 0;
  uint8_t patterns[4] = {0};

  for (carrier_iter = 0; carrier_iter < 4; carrier_iter++) {
    uint8_t bit = carrier[carrier_iter] & 0x01;
    patterns[carrier_iter] = bit;
  }

  for (uint32_t j = 0; carrier_iter < carrier_size;) {
    if (carrier_iter % 3 != 2) {
      uint8_t pattern_index = (carrier[carrier_iter] & 0x06) >> 0x01;
      uint8_t bit = carrier[carrier_iter] & 0x01;
      if (patterns[pattern_index] == 1) {
        bit = bit == 1 ? 0 : 1;
      }
      byte <<= 1;
      byte |= bit;
      if (++j % 8 == 0) {
        data_out[hidden_iter++] = byte;
        byte = 0;
        j = 0;
      }
      carrier_iter++;
    } else {
      carrier_iter++;
    }
  }
  return carrier_iter;
}
