#include "params.h"

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

Operation get_operation(const char *operation) {
  if (strcmp(operation, "-embed") == 0) {
    return EMBED;
  } else if (strcmp(operation, "-extract") == 0) {
    return EXTRACT;
  }
  return INVALID_OP;
}

int read_params(Params *params, int argc, char *argv[]) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-in") == 0 && i + 1 < argc) {
      params->input_file = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      params->carrier_bmp = argv[++i];
    } else if (strcmp(argv[i], "-out") == 0 && i + 1 < argc) {
      params->output_file = argv[++i];
    } else if (strcmp(argv[i], "-steg") == 0 && i + 1 < argc) {
      params->steg_algo = get_steg_algo(argv[++i]);
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      params->enc_algo = get_enc_algo(argv[++i]);
    } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
      params->enc_mode = get_enc_mode(argv[++i]);
    } else if (strcmp(argv[i], "-pass") == 0 && i + 1 < argc) {
      params->password = argv[++i];
    } else {
      printf("Error: Invalid argument %s\n", argv[i]);
      return -1;
    }
  }
  return 0;
}

StegAlgo get_steg_algo(const char *steg_algo) {
  if (steg_algo == NULL) {
    return INVALID_STEG_ALGO;
  }
  if (strcmp(steg_algo, "LSB1") == 0) {
    return LSB1;
  }
  if (strcmp(steg_algo, "LSB4") == 0) {
    return LSB4;
  }
  if (strcmp(steg_algo, "LSBI") == 0) {
    return LSBI;
  }
  return INVALID_STEG_ALGO;
}

EncAlgo get_enc_algo(const char *enc_algo) {
  if (enc_algo == NULL || strcmp(enc_algo, "aes128") == 0) {
    return AES128;
  }
  if (strcmp(enc_algo, "aes192") == 0) {
    return AES192;
  }
  if (strcmp(enc_algo, "aes256") == 0) {
    return AES256;
  }
  if (strcmp(enc_algo, "3des") == 0) {
    return DES3;
  }
  return INVALID_ENC_ALGO;
}

EncMode get_enc_mode(const char *enc_mode) {
  if (enc_mode == NULL || strcmp(enc_mode, "cbc") == 0) {
    return CBC;
  }
  if (strcmp(enc_mode, "cfb") == 0) {
    return CFB;
  }
  if (strcmp(enc_mode, "ofb") == 0) {
    return OFB;
  }
  if (strcmp(enc_mode, "ecb") == 0) {
    return ECB;
  }
  return INVALID_ENC_MODE;
}


lsb_func_t get_lsb_function(Operation op, StegAlgo steg_algo) {
    switch (op) {
        case EMBED:
            switch (steg_algo) {
                case LSB1:
                    return (lsb_func_t) lsb1_embed;
                case LSB4:
                    return (lsb_func_t) lsb4_embed;
                case LSBI:
                    return (lsb_func_t) lsbi_embed;
                default:
                    return NULL;
            }
        case EXTRACT:
            switch (steg_algo) {
                case LSB1:
                    return (lsb_func_t) lsb1_extract;
                case LSB4:
                    return (lsb_func_t) lsb4_extract;
                case LSBI:
                    return (lsb_func_t) lsbi_extract;
                default:
                    return NULL;
            }
        default:
            return NULL;
    }
}
