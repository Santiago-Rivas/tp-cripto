#include "lsb.h"
#include "params.h"
#include <string.h>

typedef uint32_t embed_size_t;

int embed(Params* params, CryptData* crypt_data, BMPImage* carrier);

int extract(Params* params, CryptData* crypt_data, BMPImage* carrier);

int main(int argc, char* argv[]) {
  if (argc < 6) {
    fprintf(stderr, "Error: Insufficient arguments provided.\n");
    print_usage();
    return 1;
  }

  Params params = { .operation = INVALID_OP,
                   .input_file = NULL,
                   .carrier_bmp = NULL,
                   .output_file = NULL,
                   .steg_algo = EMPTY_STEG_ALGO,
                   .enc_algo = AES128,
                   .enc_mode = CBC,
                   .password = NULL };

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
  }
  else if (params.steg_algo == INVALID_STEG_ALGO) {
    fprintf(stderr, "Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
    return 1;
  }

  if (params.enc_algo == INVALID_ENC_ALGO) {
    fprintf(stderr, "Error: Invalid Encryption Algorithm.\n");
    return 1;
  }

  if (params.enc_mode == INVALID_ENC_MODE) {
    fprintf(stderr, "Error: Invalid Encryption Mode.\n");
    return 1;
  }

  CryptData* crypt_data = NULL;
  if (params.password != NULL) {
    crypt_data = malloc(sizeof(CryptData));

    get_crypt_data(crypt_data, params.password, params.enc_algo,
      params.enc_mode);
  }

  BMPImage* image = bmp_read(params.carrier_bmp);
  if (image == NULL) {
    fprintf(stderr, "Failed to open image.\n");
    free_crypt_data(crypt_data);
    return 1;
  }

  int return_val = 0;
  if (params.operation == EMBED) {
    // fprintf(stderr, "EMBED\n");
    return_val = embed(&params, crypt_data, image);
  }
  else if (params.operation == EXTRACT) {
    // fprintf(stderr, "EXTRACT\n");
    return_val = extract(&params, crypt_data, image);
  }

  free(image);
  free_crypt_data(crypt_data);

  return return_val;
}

int embed(Params* params, CryptData* crypt_data, BMPImage* carrier) {
  long file_size;
  unsigned char* file_data = read_file_data(params->input_file, &file_size);
  if (file_data == NULL) {
    fprintf(stderr, "Failed to read file data.\n");
    return 1;
  }

  char* extension = get_file_extension(params->input_file);
  size_t ext_length = strlen(extension) + 1; // +1 for '\0'
  uint32_t file_size_be = htobe32(file_size);

  size_t buffer_size = sizeof(uint32_t) + file_size + ext_length;
  unsigned char* embed_buffer = malloc(buffer_size);
  if (embed_buffer == NULL) {
    fprintf(stderr, "Failed to allocate memory for embedding buffer.\n");
    free(file_data);
    return 1;
  }

  memcpy(embed_buffer, &file_size_be, sizeof(uint32_t));
  memcpy(embed_buffer + sizeof(uint32_t), file_data, file_size);
  memcpy(embed_buffer + sizeof(uint32_t) + file_size, extension, ext_length);

  if (crypt_data != NULL) {
    uint8_t* enc_buffer = NULL;
    long enc_buffer_len = 0;

    encrypt(crypt_data, embed_buffer, buffer_size, &enc_buffer, &enc_buffer_len);

    free(embed_buffer);
    buffer_size = sizeof(uint32_t) + enc_buffer_len;
    embed_buffer = malloc(buffer_size);

    uint32_t enc_buffer_len_be = htobe32(enc_buffer_len);

    memcpy(embed_buffer, &enc_buffer_len_be, sizeof(uint32_t));
    memcpy(embed_buffer + sizeof(uint32_t), enc_buffer, enc_buffer_len);

    free(enc_buffer);
  }

  BMPImage* output = bmp_clone(carrier);
  if (output == NULL) {
    fprintf(stderr, "Failed to clone BMP image.\n");
    free(embed_buffer);
    free(file_data);
    return 1;
  }

  size_t embed_size = get_lsb_function(params->operation, params->steg_algo)(embed_buffer, buffer_size, output->data, output->image_size);

  if (embed_size == 0) {
    fprintf(stderr, "Failed to embed all data, not enough space.\n");
    free(embed_buffer);
    free(file_data);
    free(output);
    return 1;
  }

  if (bmp_write(params->output_file, output)) {
    fprintf(stderr, "Failed to write BMP to file\n");
    free(embed_buffer);
    free(file_data);
    free(output);
    return 1;
  }

  free(embed_buffer);
  free(file_data);
  free(output);
  return 0;
}

int extract(Params* params, CryptData* crypt_data, BMPImage* carrier) {
  embed_size_t embeded_data_size = 0;
  size_t embed_size = 0;

  long image_size = carrier->header.size - 54;
  carrier->image_size = image_size;

  uint8_t* embeded_data = calloc(1, carrier->image_size);
  if (embeded_data == NULL) {
    return -1;
  }

  embed_size += get_lsb_function(params->operation, params->steg_algo)(embeded_data, carrier->image_size, carrier->data, carrier->image_size);
  embeded_data_size = be32toh(((uint32_t*)embeded_data)[0]);

  if (embeded_data_size == 0) {
    free(embeded_data);
    fprintf(stderr, "Error: Nothing to read.\n");
    return -1;
  }

  if (embeded_data_size > carrier->image_size) {
    free(embeded_data);
    fprintf(stderr, "Error: Embedded data size too big.\n");
    return -1;
  }

  uint8_t* plaintext = NULL;
  long plaintext_len = 0;
  uint32_t data_size = embeded_data_size;

  if (crypt_data != NULL) {
    if (!decrypt(crypt_data, &plaintext, &plaintext_len, embeded_data + sizeof(uint32_t), embeded_data_size)) {
      free(embeded_data);
      return -1;
    }

    if (plaintext_len > 0) {
      uint32_t raw_data_size = ((uint32_t*)plaintext)[0];
      data_size = be32toh(raw_data_size);
    }
    else {
      fprintf(stderr, "Error: Decrypted plaintext length is zero.\n");
      free(embeded_data);
      return -1;
    }
  }
  else {
    plaintext = embeded_data;
    plaintext_len = embeded_data_size;
    data_size = be32toh(((uint32_t*)plaintext)[0]);
  }

  if (data_size > (uint32_t)plaintext_len) {
    if (crypt_data != NULL) {
      free(plaintext);
    }
    free(embeded_data);
    return -1;
  }

  uint8_t* data = plaintext + sizeof(uint32_t);
  char* extension = (char*)(plaintext + sizeof(uint32_t) + data_size);

  char filename[500] = { 0 };
  // TODO: Ver este print
  snprintf(filename, sizeof(filename), "%s%s", params->output_file, extension);
  filename[254] = 0;

  // TODO: Ver este print
  printf("Output file: %s\n", filename);

  FILE* file = fopen(filename, "wb");
  if (file == NULL) {
    if (crypt_data != NULL) {
      free(plaintext);
    }
    free(embeded_data);
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
