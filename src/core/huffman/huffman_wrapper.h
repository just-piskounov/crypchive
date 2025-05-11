#ifndef HUFFMAN_WRAPPER_H
#define HUFFMAN_WRAPPER_H

#include <vector>
#include <string>
#include <unordered_map>

extern "C" {
    #include "huffman.h"
}

// Get Huffman codes for a set of data
std::unordered_map<byte, std::string> getHuffmanCodes(const std::vector<byte>& data);

// Encode data using Huffman codes
std::string encodeData(const std::vector<byte>& data, const std::unordered_map<byte, std::string>& codes);

// Decode data using Huffman codes
std::vector<byte> decodeData(const std::string& encodedData, const std::unordered_map<byte, std::string>& codes);

// Convert binary string to byte array
std::vector<byte> binaryStringToBytes(const std::string& binaryStr);

// Convert byte array to binary string
std::string bytesToBinaryString(const std::vector<byte>& bytes);

// Serialize Huffman table
std::string serializeHuffmanTable(const std::unordered_map<byte, std::string>& codes);

// Deserialize Huffman table
std::unordered_map<byte, std::string> deserializeHuffmanTable(const std::string& serialized);

#endif // HUFFMAN_WRAPPER_H
