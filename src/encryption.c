#include "encryption.h"

const char *algo_to_string(EncAlgo enc_algo);
const char *mode_to_string(EncMode enc_mode);

int encrypt(unsigned char *plaintext, long plaintext_len, const char *password,
            unsigned char **ciphertext, long *ciphertext_len, EncAlgo enc_algo,
            EncMode enc_mode)
{

  const EVP_CIPHER *cipher = get_cipher(enc_algo, enc_mode);
  if (!cipher)
  {
    printf("Error: Invalid cipher algorithm or mode.\n");
    return 0;
  }

  const int keylen = EVP_CIPHER_key_length(cipher);
  const int ivlen = EVP_CIPHER_iv_length(cipher);

  unsigned char *key_iv_pair = malloc(keylen + ivlen);
  const unsigned char salt[8] = {0}; // TODO: should this be somewhere else?

  PKCS5_PBKDF2_HMAC(password, sizeof(password), salt, sizeof(salt), 10000,
                    EVP_sha256(), keylen + ivlen, key_iv_pair);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex2(ctx, cipher, key_iv_pair, key_iv_pair + keylen, NULL);

  *ciphertext =
      (unsigned char *)malloc(plaintext_len + EVP_CIPHER_block_size(cipher));
  int len;
  int ciphertext_size;
  if (EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len) !=
      1)
  {
    printf("Error: EVP_EncryptUpdate.\n");
    return 0;
  }

  ciphertext_size = len;

  if (EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len) != 1)
  {
    printf("Error: EVP_EncryptFinal_ex.\n");
    return 0;
  }
  ciphertext_size += len;

  *ciphertext_len = ciphertext_size;

  free(key_iv_pair);
  EVP_CIPHER_CTX_free(ctx);
  EVP_CIPHER_free((EVP_CIPHER *)cipher);

  return 1;
}

// Function to derive key and IV from password
int derive_key_and_iv(const char *password, const EVP_CIPHER *cipher,
                      unsigned char *key, unsigned char *iv)
{

  int key_len = EVP_CIPHER_key_length(cipher);

  if (!PKCS5_PBKDF2_HMAC(password, strlen(password), NULL, 0, ITERATIONS,
                         EVP_sha256(), key_len, key))
  {
    return 0;
  }

  int iv_len = EVP_CIPHER_iv_length(cipher);
  if (RAND_bytes(iv, iv_len) != 1)
  {
    return 0;
  }
  return 1;
}

const EVP_CIPHER *get_cipher(EncAlgo enc_algo, EncMode enc_mode)
{
  EVP_CIPHER *cipher = NULL;
  char cipher_name[50];

  const char *algo_name = algo_to_string(enc_algo);
  const char *mode_name = mode_to_string(enc_mode);

  if (algo_name == NULL)
  {
    return NULL;
  }
  snprintf(cipher_name, sizeof(cipher_name), "%s-%s", algo_name, mode_name);
  cipher = EVP_CIPHER_fetch(NULL, cipher_name, NULL);
  return cipher;
}

const char *algo_to_string(EncAlgo enc_algo)
{
  switch (enc_algo)
  {
  case AES128:
    return "AES-128";
  case AES192:
    return "AES-192";
  case AES256:
    return "AES-256";
  case DES3:
    return "DES-EDE3";
  default:
    return NULL; // Unsupported encryption algorithm
  }
}

const char *mode_to_string(EncMode enc_mode)
{
  switch (enc_mode)
  {
  case ECB:
    return "ECB";
  case CFB:
    return "CFB";
  case OFB:
    return "OFB";
  case CBC:
    return "CBC";
  default:
    return NULL; // Unsupported encryption algorithm
  }
}