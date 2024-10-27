#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "encryption.h"
#include "bmp.h"

typedef enum { EMBED, EXTRACT, INVALID_OP } Operation;
typedef enum { LSB1, LSB4, LSBI, INVALID_STEG_ALGO, EMPTY_STEG_ALGO } StegAlgo;

typedef struct {
    Operation operation;
    const char *input_file;
    const char *carrier_bmp;
    const char *output_file;
    StegAlgo steg_algo;
    EncAlgo enc_algo;
    EncMode enc_mode;
    const char *password;
} Params;

void print_usage();

Operation get_operation(const char *operation);

int read_params(Params *params, int argc, char *argv[]);

StegAlgo get_steg_algo(const char *steg_mode);

EncAlgo get_enc_algo(const char *enc_algo);

EncMode get_enc_mode(const char *enc_mode);

# endif // PARAMS_H
