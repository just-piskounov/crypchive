#ifndef HUFFMAN_H 
#define HUFFMAN_H

#include <stdint.h>

#define MAX_TREE_HT 100000

typedef uint8_t byte;

// Minheap data structure to store the huffman tree
typedef struct MinHeapNode {
  byte data;
  unsigned freq;
  struct MinHeapNode *left, *right;
} minheapNode;

// Minheap type/data-structure
typedef struct MinHeap {
  unsigned size;
  unsigned capacity;
  minheapNode** array;
} minheap;

// Minheap related functions
minheapNode* NewNode(byte data, unsigned freq); 

minheap* createMinheap(unsigned capacity);

void swapMinheapNode(minheapNode** a, minheapNode** b);

void minHeapify(minheap* minHeap, int idx);

int isSizeOne(minheap* minHeap);

minheapNode* extractMin(minheap* minHeap);

void insertMinHeap(minheap* minHeap, minheapNode* minHeapNode);

void buildMinHeap(minheap* minHeap);

// Util functions
void printArr(int arr[], int n);

int isLeaf(minheapNode* root);

minheap* createAndbuildMinheap(byte data[], int freq[], int size);

// Huffman coding
minheapNode* buildHuffmanTree(byte data[], int freq[], int size);

void printCodes(minheapNode* root, int arr[], int top);

void HuffmanCodes(byte data[], int freq[], int size);

#endif
