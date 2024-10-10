#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_file_size(const char *filename);

char* get_file_extension(const char *filename);

unsigned char* read_file_data(const char *filename, long *size);

#endif // FILE_UTILS_H
