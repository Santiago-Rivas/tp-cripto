#include "main.h"

int main(int argc, char *argv[]) {
  if (argc < 6) {
    printf("Error: Insufficient arguments provided.\n");
    print_usage();
    return 1;
  }

  State state = {INVALID_OP,      NULL,   NULL, NULL,
                 EMPTY_STEG_ALGO, AES128, CBC,  NULL};

  state.operation = get_operation(argv[1]);

  if (state.operation == INVALID_OP) {
    printf("Error: First argument must be -embed or -extract.\n");
    print_usage();
    return 1;
  }

  if (!read_params(&state, argc, argv)) {
    print_usage();
    return 1;
  }


  if (state.steg_algo == EMPTY_STEG_ALGO) {
    printf("Error: Steganography algorithm must be provided.\n");
    return 1;
  } else if (state.steg_algo == INVALID_STEG_ALGO) {
    printf(
        "Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
    return 1;
  }

  // printf("Mode: %s\n", state.operation == EMBED ? "Embed" : "Extract");
  // printf("Input file: %s\n",
  //        state.input_file ? state.input_file : "Not provided");
  // printf("Carrier BMP: %s\n",
  //        state.carrier_bmp ? state.carrier_bmp : "Not provided");
  // printf("Output file: %s\n",
  //        state.output_file ? state.output_file : "Not provided");

  // if (state.password != NULL) {
  //   printf("Encryption with password '%s'\n", state.password);
  // } else {
  //   printf("No encryption applied.\n");
  // }

  CryptData * crypt_data = NULL;
  if (state.password != NULL) {
    crypt_data = malloc(sizeof(CryptData));
    get_crypt_data(crypt_data, state.password, state.enc_algo, state.enc_mode);
  }

  BMPImage image;
  image.data = read_bmp(state.carrier_bmp, &image);
  if (!image.data) {
    fprintf(stderr, "Failed to open image.\n");
    return 1;
  }


  int return_val = 0;
  if (state.operation == EMBED) {
    printf("EMBED\n");
    return_val = embed(&state, crypt_data, &image);
  } else if (state.operation == EXTRACT) {
    printf("Extract.\n");
    return_val = extract(&state);
  }

  free(image.data);
  free_crypt_data(crypt_data);

  return return_val;
}

void print_usage() {
  printf("Usage:\n");
  printf("To embed data:\n");
  printf("$ stegobmp -embed -in <file> -p <bitmapfile> -out <bitmapfile> -steg "
         "<LSB1|LSB4|LSBI> [-a <aes128|aes192|aes256|3des>] [-m "
         "<ecb|cfb|ofb|cbc>] [-pass <password>]\n");
  printf("To extract data:\n");
  printf("$ stegobmp -extract -p <bitmapfile> -out <file> -steg "
         "<LSB1|LSB4|LSBI> [-a <aes128|aes192|aes256|3des>] [-m "
         "<ecb|cfb|ofb|cbc>] [-pass <password>]\n");
}

Operation get_operation(char *operation) {
  if (strcmp(operation, "-embed") == 0) {
    return EMBED;
  } else if (strcmp(operation, "-extract") == 0) {
    return EXTRACT;
  }
  return INVALID_OP;
}

int read_params(State *state, int argc, char *argv[]) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-in") == 0 && i + 1 < argc) {
      state->input_file = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      state->carrier_bmp = argv[++i];
    } else if (strcmp(argv[i], "-out") == 0 && i + 1 < argc) {
      state->output_file = argv[++i];
    } else if (strcmp(argv[i], "-steg") == 0 && i + 1 < argc) {
      state->steg_algo = get_steg_algo(argv[++i]);
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      state->enc_algo = get_enc_algo(argv[++i]);
    } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
      state->enc_mode = get_enc_mode(argv[++i]);
    } else if (strcmp(argv[i], "-pass") == 0 && i + 1 < argc) {
      state->password = argv[++i];
    } else {
      printf("Error: Invalid argument %s\n", argv[i]);
      return 0;
    }
  }
  return 1;
}

StegAlgo get_steg_algo(char *steg_algo) {
  if (steg_algo == NULL) {
    return INVALID_STEG_ALGO;
  }
  if (strcmp(steg_algo, "LSB1") != 0) {
    return LSB1;
  }
  if (strcmp(steg_algo, "LSB4") != 0) {
    return LSB4;
  }
  if (strcmp(steg_algo, "LSBI") != 0) {
    return LSBI;
  }
  return INVALID_STEG_ALGO;
}

EncAlgo get_enc_algo(char *enc_algo) {
  if (enc_algo == NULL || strcmp(enc_algo, "aes128") != 0) {
    return AES128;
  }
  if (strcmp(enc_algo, "aes192") != 0) {
    return AES192;
  }
  if (strcmp(enc_algo, "aes256") != 0) {
    return AES256;
  }
  if (strcmp(enc_algo, "3des") != 0) {
    return DES3;
  }
  return INVALID_ENC_ALGO;
}

EncMode get_enc_mode(char *enc_mode) {
  if (enc_mode == NULL || strcmp(enc_mode, "cbc") != 0) {
    return CBC;
  }
  if (strcmp(enc_mode, "cfb") != 0) {
    return CFB;
  }
  if (strcmp(enc_mode, "ofb") != 0) {
    return OFB;
  }
  if (strcmp(enc_mode, "ecb") != 0) {
    return ECB;
  }
  return INVALID_ENC_MODE;
}

int embed(State *state, CryptData * crypt_data, BMPImage * carrier) {
  long file_size;
  unsigned char *file_data = read_file_data(state->input_file, &file_size);
  if (!file_data) {
    fprintf(stderr, "Failed to read file data.\n");
    return 1;
  }

  char *extension = get_file_extension(state->input_file);
  size_t ext_length = strlen(extension) + 1; // +1 for '\0'


  // Prepare data to encrypt: size of real file || file data || extension
  size_t total_size = sizeof(long) + file_size +
                      ext_length; // 4 bytes for size + file data + extension
  unsigned char *data_to_encrypt = malloc(total_size);
  if (!data_to_encrypt) {
    fprintf(stderr, "Memory allocation failed.\n");
    free(file_data);
    return 1;
  }

  memcpy(data_to_encrypt, &file_size, sizeof(long));
  memcpy(data_to_encrypt + sizeof(long), file_data, file_size);
  memcpy(data_to_encrypt + sizeof(long) + file_size, extension, ext_length);

  long data_to_embed_len = file_size;

  unsigned char *encrypted_data = NULL;

  if (crypt_data != NULL) {
    long encrypted_data_len = 0;
    if (!encrypt(crypt_data, data_to_encrypt, total_size, &encrypted_data, &encrypted_data_len)) {
      fprintf(stderr, "Encryption failed.\n");
      free(data_to_encrypt);
      free(file_data);
      return 1;
    }
    data_to_embed_len = encrypted_data_len;
  }

  if (encrypted_data == NULL) {
    encrypted_data = data_to_encrypt;
    data_to_encrypt = NULL;
  }

  size_t total_embedded_len = sizeof(long) + data_to_embed_len;
  unsigned char *data_to_embed = malloc(total_embedded_len);
  if (!data_to_embed) {
    fprintf(stderr, "Memory allocation failed.\n");
    free(encrypted_data);
    if (data_to_encrypt != NULL) {
      free(data_to_encrypt);
    }
    free(file_data);
    return 1;
  }

  memcpy(data_to_embed, &data_to_embed_len, sizeof(long));
  memcpy(data_to_embed + sizeof(long), encrypted_data, data_to_embed_len);

  if (carrier->image_size < total_embedded_len * 8) {
    fprintf(stderr, "Carrier image is too small for message.\n");
    free(encrypted_data);
    if (data_to_encrypt != NULL) {
      free(data_to_encrypt);
    }
    free(file_data);
    free(data_to_embed);
    return 1;
  }

  BMPImage output;
  output.header = carrier->header;
  output.image_size = carrier->image_size;
  output.data = malloc(carrier->image_size);

  int i = 0;
  int j = 0;
  while (i < total_embedded_len) {
    unsigned char byte = data_to_embed[i];
    for (int k = 0; k < 8; k++, j++) {
      unsigned char last_bit =
          (byte >> k) & 0x01; // shift to current bit and get it
      unsigned char in = carrier->data[j] & 0xFE; // Clear last bit
      unsigned char new =
          in | last_bit; // XOR with 0 and the last_bit = last_bit
      output.data[j] = new;
    }
    i++;
  }

  while (j < carrier->image_size) {
    output.data[j] = carrier->data[j];
    j++;
  }
  // for (int i = 0; i < sizeof(long) + file_size + ext_length; i++) {
  //   printf("DATA TO Embed: %c\n", data_to_embed[i]);
  // }

  if (!write_bmp_to_file(state->output_file, &output)) {
    fprintf(stderr, "Failed to write BMP to file\n");
    free(output.data);
    if (data_to_encrypt != NULL) {
      free(data_to_encrypt);
    }
    free(data_to_embed);
    free(file_data);
    return 1;
  }

  free(output.data);
  free(data_to_embed);
  free(file_data);
  free(encrypted_data);
  if (data_to_encrypt != NULL) {
    free(data_to_encrypt);
  }
  return 0;
}

int extract(State *state) {
  BMPImage image;
  image.data = read_bmp(state->carrier_bmp, &image);
  if (!image.data) {
    fprintf(stderr, "Failed to open carrier image.\n");
    return 1;
  }

  // Step 1: Extract the length of the embedded data
  long data_len = 0;
  int j = 0;

  for (int i = 0; i < sizeof(long); i++) {
    unsigned char byte = 0;
    for (int k = 0; k < 8; k++, j++) {
      unsigned char bit = (image.data[j] & 0x01);  // Get the LSB of each byte
      byte |= (bit << k);
    }
    ((unsigned char*)&data_len)[i] = byte;
  }

  // Step 2: Allocate memory for the extracted data
  unsigned char *extracted_data = malloc(data_len);
  if (!extracted_data) {
    fprintf(stderr, "Memory allocation failed.\n");
    free(image.data);
    return 1;
  }

  // Step 3: Extract the embedded data
  for (long i = 0; i < data_len; i++) {
    unsigned char byte = 0;
    for (int k = 0; k < 8; k++, j++) {
      unsigned char bit = (image.data[j] & 0x01);
      byte |= (bit << k);
    }
    extracted_data[i] = byte;
  }

  // Step 4: Decrypt the data if needed
  unsigned char *decrypted_data = NULL;
  long decrypted_data_len = data_len;

  if (state->password != NULL) {
    CryptData *crypt_data = malloc(sizeof(CryptData));
    if (!crypt_data) {
      fprintf(stderr, "Memory allocation failed.\n");
      free(extracted_data);
      free(image.data);
      return 1;
    }
    get_crypt_data(crypt_data, state->password, state->enc_algo, state->enc_mode);

    if (!decrypt(crypt_data, extracted_data, data_len, &decrypted_data, &decrypted_data_len)) {
      fprintf(stderr, "Decryption failed.\n");
      free_crypt_data(crypt_data);
      free(extracted_data);
      free(image.data);
      return 1;
    }
    free_crypt_data(crypt_data);
  } else {
    decrypted_data = extracted_data; // No decryption
  }

  // Step 5: Retrieve the original file size and extension
  long file_size = 0;
  memcpy(&file_size, decrypted_data, sizeof(long));
  char *extension = (char *)(decrypted_data + sizeof(long) + file_size);

  // Step 6: Write the extracted file to the output
  FILE *output_file = fopen(state->output_file, "wb");
  if (!output_file) {
    fprintf(stderr, "Failed to open output file.\n");
    if (state->password != NULL) {
        free(decrypted_data);
    }
    free(extracted_data);
    free(image.data);
    return 1;
  }

  fwrite(decrypted_data + sizeof(long), 1, file_size, output_file);
  fclose(output_file);

  // Clean up
  if (state->password != NULL) {
    free(decrypted_data);
  }
  free(extracted_data);
  free(image.data);

  return 0;
}
