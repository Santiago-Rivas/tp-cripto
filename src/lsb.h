#ifndef LSB_H
#define LSB_H

#include <stddef.h>
#include <stdint.h>

typedef enum { LSB1, LSB4, LSBI, INVALID_STEG_ALGO, EMPTY_STEG_ALGO } StegAlgo;

typedef size_t(*lsb_func_t)(uint8_t* data, size_t size, uint8_t* carrier, size_t carrier_size);

/**
 * @param[in] data bytes to embed
 * @param[in] size number of bytes to embed
 * @param[in,out] carrier bytes to hold embedded data
 * @param[in] carrier_size number of bytes to hold embedded data
 * @return embedded size on success, zero on failure (if carrier size < embedded
 * size)
 */
size_t lsb1_embed(uint8_t* data, size_t size, uint8_t* carrier,
    size_t carrier_size);

/**
 * @param[out] data buffer for extracted data
 * @param[in] size number of bytes to extract (if zero, extracts null terminated
 * string)
 * @param[in] carrier bytes holding embedded data
 * @param[in] carrier_size number of bytes holding embedded data
 * @return embedded size on success, zero on failure (if carrier size < embedded
 * size)
 */
size_t lsb1_extract(uint8_t* data_out, size_t size, const uint8_t* carrier,
    size_t carrier_size);

size_t lsb4_embed(uint8_t* data, size_t size, uint8_t* carrier,
    size_t carrier_size);

size_t lsb4_extract(uint8_t* data_out, size_t size, const uint8_t* carrier,
    size_t carrier_size);

size_t lsbi_embed(uint8_t* data, size_t size, uint8_t* carrier,
    size_t carrier_size);

size_t lsbi_extract(uint8_t* data_out, size_t size, const uint8_t* carrier,
    size_t carrier_size);

#endif // LSB_H
