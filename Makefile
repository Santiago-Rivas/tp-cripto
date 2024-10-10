# Define directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
TARGET = $(BIN_DIR)/stegobmp

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -fsanitize=address,undefined -fno-omit-frame-pointer -O1

# OpenSSL flags
OPENSSL_LIBS = -lssl -lcrypto
OPENSSL_INCLUDE = $(shell pkg-config --cflags openssl)

# Find all C files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Default target: compile the program
all: $(TARGET)

# Compilation rule for the target
$(TARGET): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(OPENSSL_LIBS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) -c $< -o $@

# Clean up
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
