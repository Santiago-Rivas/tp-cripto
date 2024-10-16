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

typedef struct {
  int keylen;
  int ivlen;
  unsigned char *key_iv_pair;
  const EVP_CIPHER *cipher;
} CryptData;

int derive_key_and_iv(const char *password, const EVP_CIPHER *cipher,
                      unsigned char *key, unsigned char *iv);

const EVP_CIPHER *get_cipher(EncAlgo enc_algo, EncMode enc_mode);

int get_crypt_data(CryptData *crypt_data, const char *password,
                   EncAlgo enc_algo, EncMode enc_mode);

void free_crypt_data(CryptData * crypt_data);
int encrypt(CryptData *crypt_data, unsigned char *plaintext, long plaintext_len,
            unsigned char **ciphertext, long *ciphertext_len);

#endif // ENCRYPTION_H
