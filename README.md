# Huffman Compression Tool

A Qt-based GUI application for compressing and decompressing files using Huffman coding.

## Features

- Text and file compression using Huffman encoding
- Decompression of Huffman-encoded files
- Visual representation of compression statistics
- Display of Huffman code table
- User-friendly Qt-based GUI

## Building the Application

### Prerequisites

- Qt 5.12 or higher
- C++11 compatible compiler (GCC, Clang, MSVC)
- qmake build system

### Build Instructions

1. Clone this repository:
   ```
   git clone https://github.com/yourusername/huffman-compression.git
   cd huffman-compression
   ```

2. Build using the provided Makefile:
   ```
   make
   ```

   This will:
   - Create a build directory
   - Run qmake to generate the project-specific Makefile
   - Compile the application

3. Run the application:
   ```
   make run
   ```

### Manual Build

If you prefer to manually build:

1. Create a build directory:
   ```
   mkdir build
   cd build
   ```

2. Run qmake:
   ```
   qmake ../huffman_compression.pro
   ```

3. Run make:
   ```
   make
   ```

4. Run the application:
   ```
   ./huffman_compression
   ```

## Usage

### Compressing Data

1. Enter text in the input field or load a file using the "Open File" button
2. Click "Compress" to generate Huffman codes and compress the data
3. View compression statistics and the Huffman code table
4. Save the compressed file using the "Save Compressed File" button

### Decompressing Data

1. Load a compressed file using the "Open Compressed File" button
2. Click "Decompress" to restore the original data
3. View decompression statistics
4. Save the decompressed data using the "Save Decompressed File" button

## File Format

Compressed files use the following format:
- Header: Serialized Huffman table for decoding
- Two newlines as separator
- Body: The compressed data

## Implementation Details

The application consists of three main components:

1. **Core Huffman Implementation (C)**: 
   - Min-heap implementation for building Huffman trees
   - Functions for generating Huffman codes

2. **C++ Wrapper**:
   - C++ interface to the C Huffman implementation
   - Functions for encoding/decoding data using Huffman codes

3. **Qt GUI**:
   - User-friendly interface for compressing and decompressing data
   - Visualization of compression statistics and Huffman codes

## License

[MIT License](LICENSE)
