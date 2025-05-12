#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

minheapNode* NewNode(byte data, unsigned freq) {

  minheapNode* temp = (minheapNode*)malloc(sizeof(minheapNode));
  if (!temp) {
    fprintf(stderr, "Error: Memory allocation failed for new node.\n");
    exit(1);
  }
  
  temp->left = temp->right = NULL;
  temp->data = data;
  temp->freq = freq;
  return temp;
}

minheap* createMinheap(unsigned capacity) {
  minheap* minHeap = (minheap*)malloc(sizeof(minheap));
  if (!minHeap) {
    fprintf(stderr, "Error: Memory allocation failed for minheap.\n");
    exit(1);
  }

  minHeap->size = 0;
  minHeap->capacity = capacity;

  minHeap->array = (minheapNode**)malloc(minHeap->capacity * sizeof(minheapNode*));
  if (!minHeap->array) {
    fprintf(stderr, "Error: Memory allocation failed for minheap array.\n");
    free(minHeap);
    exit(1);
  }

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
  if (!minHeap || minHeap->size <= 0) {
    fprintf(stderr, "Error: Attempt to extract from an empty or null minheap.\n");
    return NULL;
  }

  minheapNode* temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

void insertMinHeap(minheap* minHeap, minheapNode* minHeapNode) {
  if (!minHeap || !minHeapNode) {
    fprintf(stderr, "Error: Null pointer passed to insertMinHeap.\n");
    return;
  }

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
  if (!data || !freq || size <= 0) {
    fprintf(stderr, "Error: Invalid input to createAndbuildMinheap.\n");
    return NULL;
  }

  minheap* minHeap = createMinheap(size);
  for (int i = 0; i < size; ++i) {
    minHeap->array[i] = NewNode(data[i], freq[i]);
    if (!minHeap->array[i]) {
      fprintf(stderr, "Error: Failed to create node for data[%d].\n", i);
      free(minHeap->array);
      free(minHeap);
      exit(1);
    }
  }
  
  minHeap->size = size;
  buildMinHeap(minHeap);
  
  return minHeap;
}

minheapNode* buildHuffmanTree(byte data[], int freq[], int size) {
  if (!data || !freq || size <= 0) {
    fprintf(stderr, "Error: Invalid input to buildHuffmanTree.\n");
    return NULL;
  }

  minheapNode *left, *right, *top;
  minheap* minHeap = createAndbuildMinheap(data, freq, size);
  if (!minHeap) {
    fprintf(stderr, "Error: Failed to create and build minheap.\n");
    return NULL;
  }

  while (!isSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);
    if (!left || !right) {
      fprintf(stderr, "Error: Failed to extract nodes from minheap.\n");
      free(minHeap->array);
      free(minHeap);
      exit(1);
    }
    
    top = NewNode('$', left->freq + right->freq);
    if (!top) {
      fprintf(stderr, "Error: Failed to create internal node.\n");
      free(minHeap->array);
      free(minHeap);
      exit(1);
    }
    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  
  minheapNode* root = extractMin(minHeap);
  free(minHeap->array);
  free(minHeap);
  return root;
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
  if (!data || !freq || size <= 0) {
    fprintf(stderr, "Error: Invalid input to HuffmanCodes.\n");
    return;
  }

  minheapNode* root = buildHuffmanTree(data, freq, size);
  if (!root) {
    fprintf(stderr, "Error: Failed to build Huffman Tree.\n");
    return;
  }

  int arr[MAX_TREE_HT], top = 0;

  printCodes(root, arr, top);
}

