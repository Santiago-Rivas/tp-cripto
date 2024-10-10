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
