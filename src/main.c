#include "lsb.h"
#include "params.h"
#include <string.h>

typedef uint32_t embed_size_t;

int embed(Params *params, CryptData *crypt_data, BMPImage *carrier);

int extract(Params *params, CryptData *crypt_data, BMPImage *carrier);

int main(int argc, char *argv[]) {
  if (argc < 6) {
    fprintf(stderr, "Error: Insufficient arguments provided.\n");
    print_usage();
    return 1;
  }

  Params params = {.operation = INVALID_OP,
                   .input_file = NULL,
                   .carrier_bmp = NULL,
                   .output_file = NULL,
                   .steg_algo = EMPTY_STEG_ALGO,
                   .enc_algo = AES128,
                   .enc_mode = CBC,
                   .password = NULL};

  params.operation = get_operation(argv[1]);

  if (params.operation == INVALID_OP) {
    fprintf(stderr, "Error: First argument must be -embed or -extract.\n");
    print_usage();
    return 1;
  }

  if (read_params(&params, argc, argv)) {
    print_usage();
    return 1;
  }

  if (params.steg_algo == EMPTY_STEG_ALGO) {
    fprintf(stderr, "Error: Steganography algorithm must be provided.\n");
    return 1;
  } else if (params.steg_algo == INVALID_STEG_ALGO) {
    fprintf(
        stderr,
        "Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
    return 1;
  }

  // printf("Mode: %s\n", params.operation == EMBED ? "Embed" : "Extract");
  // printf("Input file: %s\n",
  //        params.input_file ? params.input_file : "Not provided");
  // printf("Carrier BMP: %s\n",
  //        params.carrier_bmp ? params.carrier_bmp : "Not provided");
  // printf("Output file: %s\n",
  //        params.output_file ? params.output_file : "Not provided");

  // if (params.password != NULL) {
  //   printf("Encryption with password '%s'\n", params.password);
  // } else {
  //   printf("No encryption applied.\n");
  // }

  CryptData *crypt_data = NULL;
  if (params.password != NULL) {
    crypt_data = malloc(sizeof(CryptData));

    get_crypt_data(crypt_data, params.password, params.enc_algo,
                   params.enc_mode);
  }

  BMPImage *image = bmp_read(params.carrier_bmp);
  if (image == NULL) {
    fprintf(stderr, "Failed to open image.\n");
    return 1;
  }

  int return_val = 0;
  if (params.operation == EMBED) {
    fprintf(stderr, "EMBED\n");
    return_val = embed(&params, crypt_data, image);
  } else if (params.operation == EXTRACT) {
    fprintf(stderr, "EXTRACT\n");
    return_val = extract(&params, crypt_data, image);
  }

  free(image);
  free_crypt_data(crypt_data);

  return return_val;
}

int embed(Params *params, CryptData *crypt_data, BMPImage *carrier) {
  long file_size;
  unsigned char *file_data = read_file_data(params->input_file, &file_size);
  if (file_data == NULL) {
    fprintf(stderr, "Failed to read file data.\n");
    return 1;
  }

  char *extension = get_file_extension(params->input_file);
  size_t ext_length = strlen(extension) + 1; // +1 for '\0'
  printf("EXTENSION %s\n", extension);
  printf("EXTENSION %zu\n", ext_length);

  BMPImage *output = bmp_clone(carrier);
  size_t embed_size = 0;

  printf("FILE SIZE = %ld\n", file_size);
  uint32_t file_size_be = htobe32(file_size);
  printf("FILE SIZE = %u\n", file_size_be);

  embed_size += lsb1_embed((uint8_t *)&file_size_be, sizeof(uint32_t),
                           output->data, output->image_size);
  embed_size += lsb1_embed(file_data, file_size, output->data + embed_size,
                           output->image_size - embed_size);
  embed_size +=
      lsb1_embed((uint8_t *)extension, ext_length, output->data + embed_size,
                 output->image_size - embed_size);

  // printf("Embedding file size: %lu\n", file_size);
  // printf("Embedding file data: %s\n", file_data);
  printf("Embedding extension: %s\n", extension);
  // printf("Embedding total size: %lu\n", embed_size);

  if (bmp_write(params->output_file, output)) {
    fprintf(stderr, "Failed to write BMP to file\n");
    free(output);
    free(file_data);
    return 1;
  }

  free(output);
  free(file_data);
  return 0;
}

int extract(Params *params, CryptData *crypt_data, BMPImage *carrier) {
  embed_size_t embeded_data_size = 0;
  size_t embed_size = 0;

  uint8_t *embeded_data = malloc(carrier->image_size);
  if (embeded_data == NULL) {
    return -1;
  }
  embed_size += lsb1_extract(embeded_data, carrier->image_size / 8, carrier->data, carrier->image_size);
  embeded_data_size = be32toh(((uint32_t *)embeded_data)[0]);

  uint8_t *plaintext = NULL;
  long plaintext_len = 0;
  uint32_t data_size = embeded_data_size;

  if (crypt_data != NULL) {
    if (!decrypt(crypt_data, &plaintext, &plaintext_len, embeded_data + sizeof(uint32_t), embeded_data_size)) {
      free(embeded_data);
      return -1;
    }
    data_size = be32toh(((uint32_t *)plaintext)[0]);
  } else {
    plaintext = embeded_data;
    plaintext_len = embeded_data_size;
  }

  // printf("data_size: %d\n", data_size);
  uint8_t *data = plaintext + sizeof(uint32_t);
  char *extension = (char *)(plaintext + sizeof(uint32_t) + data_size);

  char filename[300];
  snprintf(filename, sizeof(filename), "extracted_file%s", extension);

  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Error opening file for writing");
    return -1;
  }
  fwrite(data, 1, data_size, file);
  fclose(file);
  if (crypt_data != NULL) {
    free(plaintext);
  }
  free(embeded_data);
  return 0;
}
