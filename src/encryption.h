#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ITERATIONS 10000
#define MAX_KEY_LENGTH 32 // Maximum key length for AES-256
#define MAX_IV_LENGTH 16  // IV length for AES

typedef enum { AES128, AES192, AES256, DES3, INVALID_ENC_ALGO } EncAlgo;
typedef enum { ECB, CFB, OFB, CBC, INVALID_ENC_MODE } EncMode;

int derive_key_and_iv(const char *password, const EVP_CIPHER *cipher,
                      unsigned char *key, unsigned char *iv);

const EVP_CIPHER *get_cipher(EncAlgo enc_algo, EncMode enc_mode);

int encrypt(unsigned char *plaintext, int plaintext_len, const char *password,
            unsigned char **ciphertext, int *ciphertext_len, EncAlgo enc_algo,
            EncMode enc_mode);

#endif // ENCRYPTION_H
