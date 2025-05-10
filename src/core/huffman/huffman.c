#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

minheapNode* NewNode(byte data, unsigned freq){
  minheapNode* temp = (minheapNode*)malloc(sizeof(minheapNode));
  
temp->left = temp->right = NULL;
  temp->data = data;
  temp->freq = freq;
  return temp;
}

minheap* createMinheap(unsigned capacity) {
  minHeap* = (minheap*)malloc(sizeof(minheap));

  minHeap->size = 0;
  minHeap->capacity = capacity;

  minHeap->array = (minheapNode**)malloc(minHeap->capacity)*sizeof(minheapNode*);
  return minHeap:
}

void swapMinheapNode(minheapNode** a, minheapNode** b){
  minheapNode* t = *a;
  *a = *b;
  *b = *t;
}

void minHeapify(minheap* minHeap, int idx){
  int smallest = idx;
  int left = 2*idx + 1;
  int right = 2*idx +2;

  if(left<minHeap->size && minHeap->array[left]->freq<minHeap->array[smallest]->freq){
    smallest = left;
  }

  if(right<minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq){
    smallest = right;
  }

  if(smallest!=idx){
    swapMinheapNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

int isSizeOne(minheap minHeap*){
  return (minHeap->size == 1);
}

minheapNode* extractMin(minheap* minHeap){
  
  minheapNode* temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return tmp;
}

void insertMinHeap(minheap* minHeap, minheapNode* minHeapNode){
  ++minHeap->size;
  int i = minHeap->size-1;

  while(i && minHeapNode->freq < minHeap->array([(i-1)/2]->freq) {

    minHeap->array[i] = minHeap->array[(i-1)/2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}


