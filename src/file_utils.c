#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_file_size(FILE *file) {
    long prev = ftell(file);
    if (prev == -1L) {
        return -1L;
    }

    if (fseek(file, 0, SEEK_END)) {
        return -1L;
    }

    long size = ftell(file);
    if (size == -1L) {
        return -1L;
    }
    
    if (fseek(file, prev, SEEK_SET)) {
        return -1L;
    }

    return size;
}

char *get_file_extension(const char *filename) {
    char *extension = strrchr(filename, '.');
    if (extension == NULL || extension == filename) {
        return "";
    }
    
    return extension;
}

void *read_file_data(const char *filename, long *size_out) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }

    long size = get_file_size(file);
    if (size == -1L) {
        fclose(file);
        return NULL;
    }

    void *data = malloc(size);
    if (!data) {
        fclose(file);
        return NULL;
    }

    if (fread(data, size, 1, file) != 1) {
        fclose(file);
        free(data);
        return NULL;
    }

    fclose(file);
    *size_out = size;
    return data;
}
