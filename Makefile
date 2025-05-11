###########################
# Huffman Compression Tool
###########################

# Default build configuration
CXX = g++
CC = gcc
QMAKE = qmake
RM = rm -f
MKDIR = mkdir -p

# Directories
BUILD_DIR = build
SRC_DIR = src

# Default target
all: prepare_build run_qmake build

# Create build directory
prepare_build:
	$(MKDIR) $(BUILD_DIR)

# Generate Makefile from project file using qmake
run_qmake:
	cd $(BUILD_DIR) && $(QMAKE) ../huffman_compression.pro

# Build the application using the generated Makefile
build:
	$(MAKE) -C $(BUILD_DIR)

# Clean the build directory
clean:
	$(RM) -r $(BUILD_DIR)

# Run the application
run: all
	./$(BUILD_DIR)/huffman_compression

.PHONY: all prepare_build run_qmake build clean run
