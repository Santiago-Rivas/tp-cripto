#include "file_utils.h"

long get_file_size(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error: Cannot open file %s\n", filename);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fclose(file);
  return size;
}

char *get_file_extension(const char *filename) {
  char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) {
    return "";
  }
  return dot;
}

unsigned char *read_file_data(const char *filename, long *size) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error: Cannot open file %s\n", filename);
    return NULL;
  }

  *size = get_file_size(filename);
  if (*size < 0) {
    fclose(file);
    return NULL;
  }

  unsigned char *data = (unsigned char *)malloc(*size);
  if (!data) {
    printf("Error: Memory allocation failed\n");
    fclose(file);
    return NULL;
  }

  fread(data, 1, *size, file);
  fclose(file);
  return data;
}

unsigned char *read_bmp(const char *filename, BMPImage * image) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }

  // Read the BMP header
  fread(&image->header, sizeof(BMPHeader), 1, file);

  // Check if the file is a BMP file
  if (image->header.type != 0x4D42) {
    fprintf(stderr, "Error: Not a BMP file\n");
    fclose(file);
    return NULL;
  }

  // Calculate the size of the pixel data
  image->image_size = image->header.size - image->header.offset;

  // Allocate memory for the pixel data
  unsigned char *bmp_data = (unsigned char *)malloc(image->image_size);
  if (!bmp_data) {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }

  // Move to the pixel data
  fseek(file, image->header.offset, SEEK_SET);
  fread(bmp_data, 1, image->image_size, file);
  fclose(file);

  return bmp_data;
}

int write_bmp_to_file(const char* filename, BMPImage* image) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    size_t written = fwrite(&image->header, sizeof(BMPHeader), 1, file);
    if (written != 1) {
        perror("Error writing header");
        fclose(file);
        return 0;
    }

    written = fwrite(image->data, 1, image->image_size, file);
    if (written != image->image_size) {
        perror("Error writing image data");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}
