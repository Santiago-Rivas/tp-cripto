#include "params.h"
#include "lsb.h"

typedef uint32_t embed_size_t;

int embed(Params *params, CryptData * crypt_data, BMPImage * carrier);

int extract(Params *params, CryptData * crypt_data, BMPImage * carrier);

int main(int argc, char *argv[]) {
    if (argc < 6) {
        fprintf(stderr, "Error: Insufficient arguments provided.\n");
        print_usage();
        return 1;
    }

    Params params = {
        .operation = INVALID_OP,
        .input_file = NULL,
        .carrier_bmp = NULL,
        .output_file = NULL,
        .steg_algo = EMPTY_STEG_ALGO,
        .enc_algo = AES128,
        .enc_mode = CBC,
        .password = NULL
    };

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
        fprintf(stderr, "Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
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

    CryptData * crypt_data = NULL;
    if (params.password != NULL) {
        crypt_data = malloc(sizeof(CryptData));
        get_crypt_data(crypt_data, params.password, params.enc_algo, params.enc_mode);
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

int embed(Params *params, CryptData * crypt_data, BMPImage * carrier) {
    long file_size;
    unsigned char *file_data = read_file_data(params->input_file, &file_size);
    if (file_data == NULL) {
        fprintf(stderr, "Failed to read file data.\n");
        return 1;
    }

    char *extension = get_file_extension(params->input_file);
    size_t ext_length = strlen(extension) + 1; // +1 for '\0'


    // Prepare data to encrypt: size of real file || file data || extension
  /*   size_t total_size = sizeof(embed_size_t) + file_size + ext_length; // bytes for size + file data + extension
    unsigned char *data_to_encrypt = malloc(total_size);
    if (!data_to_encrypt) {
      fprintf(stderr, "Memory allocation failed.\n");
      free(file_data);
      return 1;
    }

    memcpy(data_to_encrypt, &file_size, sizeof(embed_size_t));
    memcpy(data_to_encrypt + sizeof(embed_size_t), file_data, file_size);
    memcpy(data_to_encrypt + sizeof(embed_size_t) + file_size, extension, ext_length);

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

    size_t total_embedded_len = sizeof(embed_size_t) + data_to_embed_len;
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

    memcpy(data_to_embed, &data_to_embed_len, sizeof(embed_size_t));
    memcpy(data_to_embed + sizeof(embed_size_t), encrypted_data, data_to_embed_len);

    */

    BMPImage *output = bmp_clone(carrier);
    size_t embed_size = 0;
    embed_size += lsb1_embed((uint8_t*)&file_size, sizeof(embed_size_t), output->data, output->image_size);
    embed_size += lsb1_embed(file_data, file_size, output->data + embed_size, output->image_size - embed_size);
    embed_size += lsb1_embed((uint8_t*)extension, ext_length, output->data + embed_size, output->image_size - embed_size);

    printf("Embedding file size: %lu\n", file_size);
    printf("Embedding file data: %s\n", file_data);
    printf("Embedding extension: %s\n", extension);
    printf("Embedding total size: %lu\n", embed_size);

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

int extract(Params *params, CryptData * crypt_data, BMPImage * carrier) {
    embed_size_t size = 0;

    size_t embed_size = 0;
    embed_size += lsb1_extract((uint8_t *)&size, sizeof(embed_size_t), carrier->data, carrier->image_size);

    uint8_t* buffer = malloc(size + 1);
    if (buffer == NULL) {
        return -1;
    }

    embed_size += lsb1_extract(buffer, size, carrier->data + embed_size, carrier->image_size - embed_size);
    buffer[size] = '\0';

    uint8_t extension[256] = {0};
    embed_size += lsb1_extract(extension, 0, carrier->data + embed_size, carrier->image_size - embed_size);

    printf("Extracted file size: %u\n", size);
    printf("Extracted file data: %s\n", buffer);
    printf("Extracted extension: %s\n", extension);
    printf("Extracted total size: %lu\n", embed_size);

    free(buffer);
    return 0;
}
