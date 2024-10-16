#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "encryption.h"

typedef enum { EMBED, EXTRACT, INVALID_OP } Operation;
typedef enum { LSB1, LSB4, LSBI, INVALID_STEG_ALGO, EMPTY_STEG_ALGO } StegAlgo;


typedef struct {
    Operation operation;
    char *input_file;
    char *carrier_bmp;
    char *output_file;
    StegAlgo steg_algo;
    EncAlgo enc_algo;
    EncMode enc_mode;
    const char *password;
} State;

int embed(State * state, CryptData * crypt_data, BMPImage * carrier);

int extract(State *state);

void print_usage();

Operation get_operation(char *operation);

int read_params(State * state, int argc, char *argv[]);

StegAlgo get_steg_algo(char *steg_mode);

EncAlgo get_enc_algo(char * enc_algo);

EncMode get_enc_mode(char * enc_mode);

# endif // MAIN_H
