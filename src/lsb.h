#ifndef LSB_H
#define LSB_H

#include <stdint.h>
#include <stddef.h>

/**
 * @param[in] data bytes to embed
 * @param[in] size number of bytes to embed
 * @param[in,out] carrier bytes to hold embedded data
 * @param[in] carrier_size number of bytes to hold embedded data
 * @return embedded size on success, zero on failure (if carrier size < embedded size)
 */
size_t lsb1_embed(const uint8_t* data, size_t size, uint8_t* carrier, size_t carrier_size);

/**
 * @param[out] data buffer for extracted data
 * @param[in] size number of bytes to extract (if zero, extracts null terminated string)
 * @param[in] carrier bytes holding embedded data
 * @param[in] carrier_size number of bytes holding embedded data
 * @return embedded size on success, zero on failure (if carrier size < embedded size)
 */
size_t lsb1_extract(uint8_t* data_out, size_t size, const uint8_t* carrier, size_t carrier_size);

#endif // LSB_H
