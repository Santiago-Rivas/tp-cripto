#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print usage instructions
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

int main(int argc, char *argv[]) {
  if (argc < 6) {
    printf("Error: Insufficient arguments provided.\n");
    print_usage();
    return 1;
  }

  int embed = 0;
  int extract = 0;
  char *input_file = NULL;
  char *carrier_bmp = NULL;
  char *output_file = NULL;
  char *steg_algo = NULL;
  char *algo = NULL;
  char *mode = NULL;
  char *password = NULL;

  if (strcmp(argv[1], "-embed") == 0) {
    embed = 1;
  } else if (strcmp(argv[1], "-extract") == 0) {
    extract = 1;
  } else {
    printf("Error: First argument must be -embed or -extract.\n");
    print_usage();
    return 1;
  }

  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-in") == 0 && i + 1 < argc) {
      input_file = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
      carrier_bmp = argv[++i];
    } else if (strcmp(argv[i], "-out") == 0 && i + 1 < argc) {
      output_file = argv[++i];
    } else if (strcmp(argv[i], "-steg") == 0 && i + 1 < argc) {
      steg_algo = argv[++i];
    } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
      algo = argv[++i];
    } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
      mode = argv[++i];
    } else if (strcmp(argv[i], "-pass") == 0 && i + 1 < argc) {
      password = argv[++i];
    } else {
      printf("Error: Invalid argument %s\n", argv[i]);
      print_usage();
      return 1;
    }
  }

  if (password != NULL) {
    if (algo == NULL) {
      algo = "aes128"; // Default to aes128
    }
    if (mode == NULL) {
      mode = "cbc"; // Default to CBC
    }
  }

  if (steg_algo != NULL) {
    if (strcmp(steg_algo, "LSB1") != 0 && strcmp(steg_algo, "LSB4") != 0 && strcmp(steg_algo, "LSBI") != 0) {
      printf("Error: Invalid steganography algorithm. Use LSB1, LSB4, or LSBI.\n");
      return 1;
    }
  } else {
    printf("Error: Steganography algorithm must be provided.\n");
    return 1;
  }

  printf("Mode: %s\n", embed ? "Embed" : "Extract");
  printf("Input file: %s\n", input_file ? input_file : "Not provided");
  printf("Carrier BMP: %s\n", carrier_bmp ? carrier_bmp : "Not provided");
  printf("Output file: %s\n", output_file ? output_file : "Not provided");
  printf("Steganography algorithm: %s\n", steg_algo);

  if (password != NULL) {
    printf("Encryption: %s in %s mode with password '%s'\n", algo, mode, password);
  } else {
    printf("No encryption applied.\n");
  }


  if (embed) {
    printf("Embed.\n");
  } else if (extract) {
    printf("Extract.\n");
  } else {
    print_usage();
    return 1;
  }
  // Proceed with embedding or extracting based on parsed values
  // (This part would be where the actual steganography and encryption logic is
  // implemented)

  return 0;
}
