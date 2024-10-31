#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "encryption.h"
#include "lsb.h"
#include "bmp.h"

typedef enum { EMBED, EXTRACT, INVALID_OP } Operation;

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

lsb_func_t get_lsb_function(Operation op, StegAlgo steg_algo);

# endif // PARAMS_H
