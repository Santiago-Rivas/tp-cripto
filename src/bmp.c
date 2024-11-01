#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BMPImage *bmp_read(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    fprintf(stderr, "Failed to open file.\n");
    return NULL;
  }

  // Read the BMP header
  uint8_t raw_header[sizeof(BMPHeader)];
  printf("sizeof(BMPHeader) = %zu\n", sizeof(BMPHeader));
  if (fread(&raw_header, 1, sizeof(BMPHeader), file) != sizeof(BMPHeader))
  {
    fprintf(stderr, "Failed to read header.\n");
    fclose(file);
    return NULL;
  }
  BMPHeader header;
      printf("RAW HEADER: ");
    for (size_t i = 0; i < 54; i++) {
        printf("%02X ", raw_header[i]);
    }
    printf("\n");
  memcpy(&header, raw_header, sizeof(BMPHeader));

  // Check if the file is a BMP file
  if (header.type != BMP_MAGIC)
  {
    printf("header.type = %x BMP_MAGIC = %x\n", header.type, BMP_MAGIC);
    fprintf(stderr, "Not a BMP.\n");
    fclose(file);
    return NULL;
  }

  // Calculate the size of the pixel data
  uint32_t image_size = header.size - header.offset;

  // Allocate memory for the image + pixel data
  BMPImage *image = malloc(sizeof(BMPImage) + image_size);
  if (!image)
  {
    fprintf(stderr, "Memory allocation error");
    fclose(file);
    return NULL;
  }

  image->header = header;
  image->image_size = image_size;

  // Move to the pixel data
  if (fseek(file, header.offset, SEEK_SET))
  {
    fprintf(stderr, "Failed to move to the pixel data.\n");
    fclose(file);
    free(image);
    return NULL;
  }

  printf("data_size = %d\n", image_size);
  printf("header.size = %d\n", image->header.size);
  printf("header.offset = %d\n", image->header.offset);

  if (fread(image->data, image_size, 1, file) != 1)
  {
    fprintf(stderr, "Failed to read BMP data.\n");
    fclose(file);
    free(image);
    return NULL;
  }

  fclose(file);
  return image;
}

int bmp_write(const char *filename, const BMPImage *image)
{
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    return -1;
  }

  // Write header
  if (fwrite(&image->header, sizeof(BMPHeader), 1, file) != 1)
  {
    fclose(file);
    return -1;
  }

  // Write padding zeroes between header and pixel data, if any
  size_t padding = image->header.offset - sizeof(BMPHeader);
  if (padding > 0)
  {
    char *zeroes = calloc(1, padding);
    if (zeroes == NULL)
    {
      fclose(file);
      return -1;
    }

    if (fwrite(zeroes, padding, 1, file) != 1)
    {
      fclose(file);
      free(zeroes);
      return -1;
    }

    free(zeroes);
  }

  // Write pixel data
  if (fwrite(image->data, image->image_size, 1, file) != 1)
  {
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

BMPImage *bmp_clone(const BMPImage *image)
{
  // Allocate memory for the image + pixel data
  BMPImage *clone = malloc(sizeof(BMPImage) + image->image_size);
  if (!clone)
  {
    return NULL;
  }

  // Copy image + pixel data
  memcpy(clone, image, sizeof(BMPImage) + image->image_size);

  return clone;
}
