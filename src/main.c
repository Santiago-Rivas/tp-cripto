#include "main.h"

int main(int argc, char *argv[]) {
  if (argc < 6) {
    printf("Error: Insufficient arguments provided.\n");
    print_usage();
    return 1;
  }

  State state = {INVALID_OP, NULL, NULL, NULL, EMPTY_STEG_ALGO, AES128, CBC, NULL};

  state.operation = get_operation(argv[1]);
  if (!read_params(&state, argc, argv)) {
      print_usage();
      return 1;
  }

  if (state.operation == INVALID_OP) {
    printf("Error: First argument must be -embed or -extract.\n");
    print_usage();
    return 1;
  }


  if (state.steg_algo == EMPTY_STEG_ALGO) {
    printf("Error: Steganography algorithm must be provided.\n");
    return 1;
  } else if (state.steg_algo == INVALID_STEG_ALGO) {
    printf( "Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
    return 1;
  }

  printf("Mode: %s\n", state.operation == EMBED ? "Embed" : "Extract");
  printf("Input file: %s\n", state.input_file ? state.input_file : "Not provided");
  printf("Carrier BMP: %s\n", state.carrier_bmp ? state.carrier_bmp : "Not provided");
  printf("Output file: %s\n", state.output_file ? state.output_file : "Not provided");

  if (state.password != NULL) {
    printf("Encryption with password '%s'\n", state.password);
  } else {
    printf("No encryption applied.\n");
  }

  if (state.operation == EMBED) {
    long file_size;
    unsigned char *file_data = read_file_data(state.input_file, &file_size);
    if (!file_data) {
        fprintf(stderr, "Failed to read file data.\n");
        return 1;
    }

    char *extension = get_file_extension(state.input_file);
    size_t ext_length = strlen(extension) + 1; // +1 for '\0'

    // Prepare data to encrypt: size of real file || file data || extension
    size_t total_size = sizeof(long) + file_size + ext_length; // 4 bytes for size + file data + extension
    unsigned char *data_to_encrypt = malloc(total_size);
    if (!data_to_encrypt) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(file_data);
        return 1;
    }

    memcpy(data_to_encrypt, &file_size, sizeof(long));
    memcpy(data_to_encrypt + sizeof(long), file_data, file_size);
    memcpy(data_to_encrypt + sizeof(long) + file_size, extension, ext_length);

    unsigned char *data_to_embed = data_to_encrypt;
    unsigned char *encrypted_data = NULL;
    if (state.password != NULL) {
      printf("ENC\n");


      int encrypted_data_len = 0;
      if (!encrypt(data_to_encrypt, total_size, state.password, &encrypted_data, &encrypted_data_len, state.enc_algo, state.enc_mode)) {
        fprintf(stderr, "Encryption failed.\n");
        free(data_to_encrypt);
        free(file_data);
        return 1;
      }
      data_to_embed = encrypted_data;
    }
    for (int i = 0 ; i < sizeof(long) + file_size + ext_length ; i++) {
      printf("DATA TO Embed: %c\n", data_to_embed[i]);
    }


    // Here, you would embed the encrypted data into the carrier image
    // For example: embed(encrypted_data, encrypted_data_len, carrier_image);

    // Free allocated memory
    free(data_to_encrypt);
    free(file_data);
    if (encrypted_data != NULL) {
      free(encrypted_data);
    }
    printf("EMBED\n");
  } else if (state.operation == EXTRACT) {
    printf("Extract.\n");
  }

  return 0;
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

int read_params(State * state, int argc, char *argv[]) {
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
  if (strcmp(steg_algo, "LSB1") != 0) {
    return LSB1;
  }
  if (strcmp(steg_algo, "LSB4") != 0) {
    return LSB4;
  }
  if (strcmp(steg_algo, "LSBI") != 0) {
  }
  return INVALID_STEG_ALGO;
}

EncAlgo get_enc_algo(char * enc_algo) {
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

EncMode get_enc_mode(char * enc_mode) {
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

