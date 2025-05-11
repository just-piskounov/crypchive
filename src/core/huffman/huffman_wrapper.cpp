#include "huffman_wrapper.h"
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>

extern "C" {
    #include "huffman.h"
}

// Structure to hold Huffman codes
struct HuffmanCode {
    std::string code;
    byte symbol;
};

// Map to store symbol -> code mappings
static std::unordered_map<byte, std::string> huffmanCodeMap;

// Helper function to collect codes during traversal
void collectCodes(minheapNode* root, std::string code) {
    if (!root)
        return;
    
    if (isLeaf(root)) {
        huffmanCodeMap[root->data] = code;
    }
    
    collectCodes(root->left, code + "0");
    collectCodes(root->right, code + "1");
}

// C++ wrapper function to get Huffman codes
std::unordered_map<byte, std::string> getHuffmanCodes(const std::vector<byte>& data) {
    // Count frequencies
    std::unordered_map<byte, int> freqMap;
    for (byte b : data) {
        freqMap[b]++;
    }
    
    // Convert to arrays for C function
    int size = freqMap.size();
    byte* symbols = new byte[size];
    int* frequencies = new int[size];
    
    int i = 0;
    for (const auto& pair : freqMap) {
        symbols[i] = pair.first;
        frequencies[i] = pair.second;
        i++;
    }
    
    // Build Huffman tree
    minheapNode* root = buildHuffmanTree(symbols, frequencies, size);
    
    // Clean up previous codes
    huffmanCodeMap.clear();
    
    // Collect codes
    collectCodes(root, "");
    
    // Free memory
    delete[] symbols;
    delete[] frequencies;
    
    return huffmanCodeMap;
}

// Function to encode data using Huffman codes
std::string encodeData(const std::vector<byte>& data, const std::unordered_map<byte, std::string>& codes) {
    std::string encodedData;
    for (byte b : data) {
        encodedData += codes.at(b);
    }
    return encodedData;
}

// Function to decode Huffman-encoded data
std::vector<byte> decodeData(const std::string& encodedData, const std::unordered_map<byte, std::string>& codes) {
    // Create a reverse mapping (code -> symbol)
    std::unordered_map<std::string, byte> reverseMap;
    for (const auto& pair : codes) {
        reverseMap[pair.second] = pair.first;
    }
    
    std::vector<byte> decodedData;
    std::string currentCode;
    
    for (char bit : encodedData) {
        currentCode += bit;
        auto it = reverseMap.find(currentCode);
        if (it != reverseMap.end()) {
            decodedData.push_back(it->second);
            currentCode.clear();
        }
    }
    
    return decodedData;
}

// Function to convert binary string to byte array
std::vector<byte> binaryStringToBytes(const std::string& binaryStr) {
    std::vector<byte> bytes;
    for (size_t i = 0; i < binaryStr.length(); i += 8) {
        std::string byteStr = binaryStr.substr(i, 8);
        // Pad with zeros if necessary
        while (byteStr.length() < 8) {
            byteStr += '0';
        }
        byte b = 0;
        for (int j = 0; j < 8; j++) {
            if (byteStr[j] == '1') {
                b |= (1 << (7 - j));
            }
        }
        bytes.push_back(b);
    }
    return bytes;
}

// Function to convert byte array to binary string
std::string bytesToBinaryString(const std::vector<byte>& bytes) {
    std::string binaryStr;
    for (byte b : bytes) {
        for (int i = 7; i >= 0; i--) {
            binaryStr += ((b >> i) & 1) ? '1' : '0';
        }
    }
    return binaryStr;
}

// Function to serialize the Huffman code table
std::string serializeHuffmanTable(const std::unordered_map<byte, std::string>& codes) {
    std::stringstream ss;
    
    // Write the number of entries
    ss << codes.size() << "\n";
    
    // Write each entry
    for (const auto& pair : codes) {
        ss << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }
    
    return ss.str();
}

// Function to deserialize the Huffman code table
std::unordered_map<byte, std::string> deserializeHuffmanTable(const std::string& serialized) {
    std::unordered_map<byte, std::string> codes;
    std::stringstream ss(serialized);
    
    int numEntries;
    ss >> numEntries;
    
    for (int i = 0; i < numEntries; i++) {
        int symbol;
        std::string code;
        ss >> symbol >> code;
        codes[static_cast<byte>(symbol)] = code;
    }
    
    return codes;
}
