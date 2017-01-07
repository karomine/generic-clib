/*
Generic link based Binary Search Tree (BST)
Written by - Kevin Romine
language - C
*/

#ifndef _BINARY_TREE_LINK_H_
#define _BINARY_TREE_LINK_H_

#include <stdlib.h>

struct BinarySearchTreeLink 
{
    struct Node * root;
    int elementSize;
    int(*compareFunction)(const void *, const void *);
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
};

// adds element to binary search tree, pass element by address
void addBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element);

// removes a node (if one exists) that matches element based on comparisonFunction
void removeBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element);

// return true if tree contains item that matches element based on comparisonFunction
int containsBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element);

// free all dynamic memory in tree and free BST structure 
void releaseBinarySearchTreeLink(struct BinarySearchTreeLink * tree);

// create BST with properties suiting a certain data type
struct BinarySearchTreeLink * makeBinarySearchTreeLink(int elementSize,
    int(*compareFunction)(const void *, const void *),
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

void printTree(struct BinarySearchTreeLink * tree);

#endif
