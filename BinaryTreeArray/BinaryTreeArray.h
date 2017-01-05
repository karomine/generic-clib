/*
Generic array based binary tree implementation
Written by - Kevin Romine
language - C
*/

#ifndef _BINARY_TREE_ARRAY_H_
#define _BINARY_TREE_ARRAY_H_

#include <stdlib.h>

/*
** tree, root, currentSize, and maxSize managed internally
** elementSize, startSize, compareFunction, copyFunction, and freeFunction provided by caller in makeBinarySearchTreeArray
** elementSize - size of data type put on BST
** startSize - starting size of BST
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copie sby passing it in
** freeFunction
    ** how to release any dynamically allocated memory from copy function
    ** pass NULL if deallocation not needed
** compareFunction
    ** function to compare the ordering of two elements
    ** return -1 arg1 < arg2, 0 for arg1 == arg2, 1 for arg1 > arg2
*/

struct BinarySearchTreeArray {
    struct Node * tree;
    int elementSize;
    int root;
    int(*comparisonFunction)(const void *, const void *, size_t);
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
    int currentSize;
    int maxSize;
};

// adds element to binary search tree, pass element by address
void addBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element); 

// removes a node (if one exists) that matches element based on comparisonFunction
void removeBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element); 

// return true if tree contains item that matches element based on comparisonFunction
int containsBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element); 

// free all dynamic memory in tree and free BST structure 
void releaseBinarySearchTreeArray(struct BinarySearchTreeArray * tree); 

// create BST with properties suiting a certain data type
struct BinarySearchTreeArray * makeBinarySearchTreeArray(int elementSize, int startSize,
    int(*compareFunction)(const void *, const void *, size_t),
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

#endif
