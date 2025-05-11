#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

minheapNode* NewNode(byte data, unsigned freq) {
  minheapNode* temp = (minheapNode*)malloc(sizeof(minheapNode));
  
  temp->left = temp->right = NULL;
  temp->data = data;
  temp->freq = freq;
  return temp;
}

minheap* createMinheap(unsigned capacity) {
  minheap* minHeap = (minheap*)malloc(sizeof(minheap));

  minHeap->size = 0;
  minHeap->capacity = capacity;

  minHeap->array = (minheapNode**)malloc(minHeap->capacity * sizeof(minheapNode*));
  return minHeap;
}

void swapMinheapNode(minheapNode** a, minheapNode** b) {
  minheapNode* t = *a;
  *a = *b;
  *b = t;
}

void minHeapify(minheap* minHeap, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
    smallest = left;
  }

  if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
    smallest = right;
  }

  if (smallest != idx) {
    swapMinheapNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

int isSizeOne(minheap* minHeap) {
  return (minHeap->size == 1);
}

minheapNode* extractMin(minheap* minHeap) {
  minheapNode* temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

void insertMinHeap(minheap* minHeap, minheapNode* minHeapNode) {
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->freq < minHeap->array[(i-1)/2]->freq) {
    minHeap->array[i] = minHeap->array[(i-1)/2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(minheap* minHeap) {
  int n = minHeap->size - 1;
  int i;

  for (i = (n-1)/2; i >= 0; --i) {
    minHeapify(minHeap, i);
  }
}

void printArr(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i) {
    printf("%d", arr[i]);
  }
  printf("\n");
}

int isLeaf(minheapNode* root) {
  return !(root->left) && !(root->right);
}

minheap* createAndbuildMinheap(byte data[], int freq[], int size) {
  minheap* minHeap = createMinheap(size);
  
  for (int i = 0; i < size; ++i) {
    minHeap->array[i] = NewNode(data[i], freq[i]);
  }
  
  minHeap->size = size;
  buildMinHeap(minHeap);
  
  return minHeap;
}

minheapNode* buildHuffmanTree(byte data[], int freq[], int size) {
  minheapNode *left, *right, *top;
  minheap* minHeap = createAndbuildMinheap(data, freq, size);

  while (!isSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);
    
    // '$' is used as an internal node marker
    top = NewNode('$', left->freq + right->freq);
    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  
  return extractMin(minHeap);
}

void printCodes(minheapNode* root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printCodes(root->left, arr, top + 1);
  }

  if (root->right) {
    arr[top] = 1;
    printCodes(root->right, arr, top + 1);
  }

  if (isLeaf(root)) {
    printf("%c: ", root->data);
    printArr(arr, top);
  }
}

void HuffmanCodes(byte data[], int freq[], int size) {
    minheapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

