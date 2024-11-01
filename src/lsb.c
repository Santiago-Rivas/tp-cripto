#include "lsb.h"

#include <stdio.h>

#define LSBI_PATTERNS 4
#define LSBI_OPERATIONS 8
#define LSB1_MASK 0xFE // Mask for setting LSB1 to 0

typedef struct
{
  uint32_t changed;
  uint32_t not_changed;
} bit_modified;

static uint8_t get_i_nibble(uint8_t byte, uint8_t i) {
    return (byte >> ((i - 1) * 4)) & 0xF;
}

// Helper function to get the pattern index based on specific bits in a byte
static uint8_t get_pattern_index(uint8_t byte)
{
  return (byte & 0x06) >> 1; // Extract bits 1 and 2 for pattern index
}

// Gets the i-th bit (1-based, 1 is LSB) from a byte
static uint8_t get_i_bit(uint8_t byte, uint8_t i)
{
  return (byte >> (i - 1)) & 1;
}

// Sets the least significant bit of a byte
static void set_ls_bit(uint8_t *byte, uint8_t value)
{
  *byte = (*byte & LSB1_MASK) | value;
}

size_t lsb1_embed(uint8_t *data, size_t size, uint8_t *carrier,
                  size_t carrier_size)
{
  if (size * 8 > carrier_size)
  {
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size; byte_i++)
  {
    for (size_t bit_i = 0; bit_i < 8; bit_i++)
    {
      size_t embed_i = byte_i * 8 + bit_i;

      uint8_t data_bit = (data[byte_i] >> (7 - bit_i)) & 0x01;

      carrier[embed_i] = (carrier[embed_i] & 0xFE) | data_bit;
      embed_size++;
    }
  }

  return embed_size;
}

size_t lsb1_extract(uint8_t *data_out, size_t size, const uint8_t *carrier, size_t carrier_size)
{
  size = carrier_size / 8;
  if (size * 8 > carrier_size)
  {
    printf("size = %zu carrier_size = %zu too big!\n", size, carrier_size);
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size || size == 0; byte_i++)
  {
    data_out[byte_i] = 0;

    for (size_t bit_i = 0; bit_i < 8; bit_i++)
    {
      size_t embed_i = byte_i * 8 + bit_i;

      uint8_t data_bit = carrier[embed_i] & 0x01;
      data_out[byte_i] <<= 1;
      data_out[byte_i] |= data_bit;
      embed_size++;
    }
    // printf("Hexadecimal (uppercase): %c %X\n", data_out[byte_i],
    // data_out[byte_i]);  // Output: FF printf("Hexadecimal (uppercase): %X\n",
    // data_out[byte_i]);  // Output: FF

    if (size == 0 && data_out[byte_i] == '\0')
    {
      break;
    }
  }

  return embed_size;
}

size_t lsb4_embed(uint8_t *data, size_t size, uint8_t *carrier, size_t carrier_size)
{
  if (size * 2 > carrier_size)
  {
    return 0;
  }

  size_t embed_size = 0;

  for (size_t byte_i = 0; byte_i < size; byte_i++)
  {
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

size_t lsb4_extract(uint8_t *data_out, size_t size, const uint8_t *carrier, size_t carrier_size) {
    size = carrier_size / 2;
    //printf("[Func1] Initial: size = %zu carrier_size = %zu\n", size, carrier_size);

    if (size * 2 > carrier_size) {
        //printf("[Func1] Error: size * 2 (%zu) > carrier_size (%zu)\n", size * 2, carrier_size);
        return 0;
    }

    size_t embed_size = 0;
    uint32_t hidden_iter = 0;
    uint8_t byte = 0;
    uint32_t i = 0, j = 0;

    //printf("[Func1] Starting extraction loop\n");
    for (i = 0, j = 0; i < carrier_size; i++) {
        if (i * 2 >= carrier_size) {
            //printf("[Func1] Break: i * 2 (%u) >= carrier_size (%zu)\n", i * 2, carrier_size);
            break;
        }

        uint8_t nibble = get_i_nibble(carrier[i], 1);
        //printf("[Func1] i=%u, carrier[i]=0x%02X, nibble=0x%X\n", i, carrier[i], nibble);

        byte <<= 4;
        byte |= nibble;
        //printf("[Func1] After shift/or: byte=0x%02X, j=%u\n", byte, j);

        if (++j % 2 == 0) {
            //printf("[Func1] Storing byte 0x%02X at position %u\n", byte, hidden_iter);
            data_out[hidden_iter++] = byte;
            byte = 0;
            j = 0;
        }

        if (size == 0 && data_out[i] == '\0') {
            //printf("[Func1] Break: Null terminator found at position %u\n", i);
            break;
        }
    }
    
    //printf("[Func1] Final: processed %u bytes, produced %u output bytes\n", i, hidden_iter);
    return i;
}

size_t lsbi_embed(uint8_t *data, size_t size, uint8_t *carrier, size_t carrier_size)
{
  if (size * 8 > carrier_size)
  {
    return 0; // Return 0 if the data is too large to embed in the carrier
  }

  bit_modified bits_modified[LSBI_PATTERNS] = {0};
  uint8_t inversion_bits[LSBI_PATTERNS] = {0};
  size_t carrier_index = 0;

  // First pass: Determine bit changes and set up inversion bits
  for (size_t byte_i = 0; byte_i < size; byte_i++)
  {
    for (int bit_i = LSBI_OPERATIONS; bit_i > 0; bit_i--)
    {
      if (carrier_index % 3 != 2)
      { // Skip every 3rd byte
        uint8_t hide_bit = get_i_bit(data[byte_i], bit_i);
        uint8_t carrier_bit = get_i_bit(carrier[carrier_index], 1);
        uint8_t pattern_index = get_pattern_index(carrier[carrier_index++]);

        if (hide_bit == carrier_bit)
        {
          bits_modified[pattern_index].not_changed++;
        }
        else
        {
          bits_modified[pattern_index].changed++;
        }
      }
      else
      {
        carrier_index++;
        bit_i++; // Adjust bit index to retry the same hide bit in the next non-skipped carrier byte
      }
    }
  }

  // Determine inversion bits based on change counts
  for (int i = 0; i < LSBI_PATTERNS; i++)
  {
    inversion_bits[i] = bits_modified[i].changed > bits_modified[i].not_changed ? 1 : 0;
  }

  // Embed inversion bits at the start of the carrier
  size_t embed_size = 0;
  for (size_t j = 0; j < LSBI_PATTERNS && embed_size < carrier_size; j++)
  {
    set_ls_bit(carrier + embed_size++, inversion_bits[j]);
  }

  // Second pass: Embed actual data with inversion based on pattern
  for (size_t byte_i = 0; byte_i < size && embed_size < carrier_size; byte_i++)
  {
    for (int bit_i = LSBI_OPERATIONS; bit_i > 0 && embed_size < carrier_size; bit_i--)
    {
      if (embed_size % 3 != 2)
      { // Skip every 3rd byte again
        uint8_t bit = get_i_bit(data[byte_i], bit_i);
        uint8_t pattern_index = get_pattern_index(carrier[embed_size]);
        set_ls_bit(carrier + embed_size++, inversion_bits[pattern_index] == 1 ? !bit : bit);
      }
      else
      {
        embed_size++; // Move to the next carrier byte without embedding
        bit_i++;      // Adjust bit index to retry the same hide bit
      }
    }
  }

  return embed_size;
}

size_t lsbi_extract(uint8_t *data_out, size_t size, const uint8_t *carrier, size_t carrier_size)
{
  size = ((carrier_size - 4) / 8) * 2 / 3;

  if (size * 8 > carrier_size)
  {
    printf("size = %zu carrier_size = %zu too big!\n", size, carrier_size);
    return 0;
  }

  uint8_t byte = 0;
  uint32_t hidden_iter = 0;
  uint32_t carrier_iter = 0;
  uint8_t patterns[4] = {0};

  for (carrier_iter = 0; carrier_iter < 4; carrier_iter++)
  {
    uint8_t bit = carrier[carrier_iter] & 0x01;
    patterns[carrier_iter] = bit;
  }

  for (uint32_t j = 0; carrier_iter < carrier_size;)
  {
    if (carrier_iter % 3 != 2)
    {
      uint8_t pattern_index = (carrier[carrier_iter] & 0x06) >> 0x01;
      uint8_t bit = carrier[carrier_iter] & 0x01;
      if (patterns[pattern_index] == 1)
      {
        bit = bit == 1 ? 0 : 1;
      }
      byte <<= 1;
      byte |= bit;
      if (++j % 8 == 0)
      {
        data_out[hidden_iter++] = byte;
        byte = 0;
        j = 0;
      }
      carrier_iter++;
    }
    else
    {
      carrier_iter++;
    }
  }
  return carrier_iter;
}
