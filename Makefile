# Define directories
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/stegobmp

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Find all C files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Default target: compile the program
all: $(TARGET)

# Compilation rule
$(TARGET): $(SRC_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET)

# Clean up
clean:
	rm -rf $(BIN_DIR)

