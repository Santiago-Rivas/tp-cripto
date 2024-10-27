#ifndef BMP_H
#define BMP_H

#define BMP_MAGIC 0x4D42

#include <stdint.h>

#pragma pack(push, 1)  // Ensure no padding is added to structures

// BMP header structure
typedef struct {      // Total: 54 bytes
    uint16_t type;              // Magic identifier: 0x4d42
    uint32_t size;              // File size in bytes
    uint16_t reserved1;         // Not used
    uint16_t reserved2;         // Not used
    uint32_t offset;            // Offset to image data in bytes from beginning of file
    uint32_t dib_header_size;   // DIB Header size in bytes (40 bytes)
    int32_t width_px;           // Width of the image
    int32_t height_px;          // Height of image
    uint16_t num_planes;        // Number of color planes
    uint16_t bits_per_pixel;    // Bits per pixel
    uint32_t compression;       // Compression type
    uint32_t image_size_bytes;  // Image size in bytes
    int32_t x_resolution_ppm;   // Pixels per meter
    int32_t y_resolution_ppm;   // Pixels per meter
    uint32_t num_colors;        // Number of colors
    uint32_t important_colors;  // Important colors
} BMPHeader;

typedef struct {
    BMPHeader header;
    long image_size;
    unsigned char data[0];
} BMPImage;

#pragma pack(pop)

/**
 * @brief Reads .bmp image from file
 * 
 * @param[in] filename 
 * @return pointer to allocated BMPImage structure, NULL on failure
 */
BMPImage *bmp_read(const char *filename);

/**
 * @brief Clones existing BMPImage
 * 
 * @param[in] image image to clone
 * @return pointer to allocated BMPImage structure, NULL on failure
 */
BMPImage *bmp_clone(const BMPImage *image);

/**
 * @brief Writes .bmp image to file
 * 
 * @param[in] filename 
 * @param[in] image 
 * @return 0 on success, -1 on failure
 */
int bmp_write(const char *filename, const BMPImage *image);

#endif // BMP_H
