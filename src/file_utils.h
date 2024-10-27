#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

/**
 * @brief Get size from open file, recovers previous position
 * 
 * @param[in] file open file
 * @return file size on success, -1L on failure
 */
long get_file_size(FILE *file);

/**
 * @brief Get pointer to file extension, references filename
 * 
 * @param[in] filename 
 * @return file extension string including '.', empty string if not found
 */
char *get_file_extension(const char *filename);

/**
 * @brief Reads all bytes from a file
 * 
 * @param[in] filename 
 * @param[out] size_out returns size of the buffer
 * @return allocated buffer containing all bytes from file, NULL on failure
 */
void *read_file_data(const char *filename, long *size_out);

#endif // FILE_UTILS_H
