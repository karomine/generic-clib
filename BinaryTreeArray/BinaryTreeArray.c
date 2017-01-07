#include "BinaryTreeArray.h"
#include <stdio.h>

struct Node 
{
    void * element;
    unsigned int leftChild;
    unsigned int rightChild;
    unsigned int parent; 
};

static int isEmpty(struct BinarySearchTreeArray * tree)
{
    return tree->root == -1; 
}

static int isLeafNode(struct BinarySearchTreeArray * tree, int nodeNum)
{
    return tree->tree[nodeNum].leftChild == -1 && tree->tree[nodeNum].rightChild == -1;
}

struct BinarySearchTreeArray * makeBinarySearchTreeArray(int elementSize, int startSize,
    int(*comparisonFunction)(const void *, const void *),
    void * (*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct BinarySearchTreeArray * tree = malloc(sizeof *tree);
    tree->tree = malloc(startSize * sizeof *tree->tree);
    tree->elementSize = elementSize;
    tree->root = -1;
    tree->comparisonFunction = comparisonFunction;
    tree->copyFunction = copyFunction;
    tree->freeFunction = freeFunction;
    tree->currentSize = 0;
    tree->currentSize = 0;
    tree->maxSize = startSize;
    return tree;
}

static void setSize(struct BinarySearchTreeArray * tree, int size)
{
    tree->maxSize = size;
    tree->tree = realloc(tree->tree, tree->maxSize * sizeof(struct Node));
}

static void shrink(struct BinarySearchTreeArray * tree)
{
    setSize(tree, tree->maxSize / 2);
}

static void grow(struct BinarySearchTreeArray * tree)
{
    printf("growing\n");
    setSize(tree, tree->maxSize * 2);
}

static void swap(struct BinarySearchTreeArray * tree, int pos1, int pos2)
{
    //printf("swapping %d with %d\n", pos1, pos2);
    void * temp = tree->tree[pos1].element;
    tree->tree[pos1].element = tree->tree[pos2].element;
    tree->tree[pos2].element = temp;
}

static void removeNode(struct BinarySearchTreeArray *, int, int);

static int searchRight(struct BinarySearchTreeArray * tree, int nodeNum, int swapWith, int parent)
{
    while (tree->tree[nodeNum].rightChild != -1)
    {
        parent = nodeNum;
        nodeNum = tree->tree[nodeNum].rightChild;
    }
    swap(tree, nodeNum, swapWith);
    removeNode(tree, nodeNum, parent);
}

static int searchLeft(struct BinarySearchTreeArray * tree, int nodeNum, int swapWith, int parent)
{
    while (tree->tree[nodeNum].leftChild != -1)
    {
        parent = nodeNum;
        nodeNum = tree->tree[nodeNum].leftChild;
    }
    swap(tree, nodeNum, swapWith);
    removeNode(tree, nodeNum, parent);
}

static void changeParentsChild(struct BinarySearchTreeArray * tree, int oldNum, int newNum, int parent)
{
    //printf("setting node %d to point to %d not %d\n", parent, newNum, oldNum);
    //printf("current size is %d\n", tree->currentSize);
    struct Node * parentNode = tree->tree + parent; 
    if (tree->currentSize == 0)
    {
        tree->root = -1;
    }
    else if (parentNode->leftChild == oldNum)
    {
        //printf("%d now points to %d not %d\n", parent, newNum, oldNum);
        parentNode->leftChild = newNum;
    }
    else
    {
        //printf("%d now points to %d not %d\n", parent, newNum, oldNum);
        parentNode->rightChild = newNum; 
    }
}

static void removeNode(struct BinarySearchTreeArray * tree, int nodeNum, int parent)
{
    //printf("removeing node %d\n", nodeNum);
    if (isLeafNode(tree, nodeNum))
    {
        tree->freeFunction(tree->tree[nodeNum].element);
        tree->currentSize--; 
        changeParentsChild(tree, nodeNum, -1, parent);
        //printf("node num is %d and currentSize is %d\n", nodeNum, tree->currentSize);
        //printf("node num is %d current size is %d\n", nodeNum, tree->currentSize);
        if (nodeNum != tree->currentSize && tree->currentSize > 0)
        {
            changeParentsChild(tree, tree->currentSize, nodeNum, tree->tree[tree->currentSize].parent);
            tree->tree[nodeNum] = tree->tree[tree->currentSize];
            //printf("swapping %d with %d\n", nodeNum, tree->currentSize);
        }
    }
    else if (tree->tree[nodeNum].leftChild == -1)
    {
        searchLeft(tree, tree->tree[nodeNum].rightChild, nodeNum, nodeNum);
    }
    else
    {
        searchRight(tree, tree->tree[nodeNum].leftChild, nodeNum, nodeNum); 
    }
}

static void removeRecurse(struct BinarySearchTreeArray * tree, int nodeNum, void * element, int parent)
{
    if (nodeNum != -1)
    {
        int comparison = tree->comparisonFunction(element, tree->tree[nodeNum].element);
        if (comparison == 0)
        {
            removeNode(tree, nodeNum, parent);
        }
        else if (comparison > 0)
        {
            removeRecurse(tree, tree->tree[nodeNum].rightChild, element, nodeNum);
        }
        else
        {
            removeRecurse(tree, tree->tree[nodeNum].leftChild, element, nodeNum);
        }
    }
}

void removeBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element)
{
    removeRecurse(tree, tree->root, element, -1);
}

static void setNode(struct BinarySearchTreeArray * tree, void * element, int nodeNum, int parent) 
{   
    int i;
    tree->copyFunction(&i, element, tree->elementSize);


    //printf("adding %d at current size %d with parent %d\n", i, nodeNum, parent);
    struct Node * node = tree->tree + nodeNum;
    node->element = malloc(tree->elementSize);
    tree->copyFunction(node->element, element, tree->elementSize);
    node->leftChild = -1;
    node->rightChild = -1;
    node->parent = parent; 
}

static int addRecurse(struct BinarySearchTreeArray * tree, void * element, int nodeNum, int parent)
{
    if (nodeNum == -1)
    {
        nodeNum = tree->currentSize++;
        setNode(tree, element, nodeNum, parent);
    }
    else
    {
        struct Node * node = tree->tree + nodeNum; 
        int comparison = tree->comparisonFunction(element, node->element);
        if (comparison >= 0)
        {
            node->rightChild = addRecurse(tree, element, node->rightChild, nodeNum);
        }
        else
        {
            node->leftChild = addRecurse(tree, element, node->leftChild, nodeNum);
        }
    }
    return nodeNum; 
}

void addBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element)
{
    if (tree->currentSize == tree->maxSize)
    {
        grow(tree);
    }
    tree->root = addRecurse(tree, element, tree->root, -1);
}

static int containsRecurse(int nodeNum, struct BinarySearchTreeArray * tree, void * elem)
{
    if (nodeNum == -1)
    {
        return 0;
    }
    else
    {
        int comparison = tree->comparisonFunction(elem, tree->tree[nodeNum].element);
        if (comparison == 0)
        {
            return 1;
        }
        else if (comparison > 0)
        {
            return containsRecurse(tree->tree[nodeNum].rightChild, tree, elem);
        }
        else
        {
            return containsRecurse(tree->tree[nodeNum].leftChild, tree, elem);
        }
    }
}

int containsBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * elem)
{
    return containsRecurse(tree->root, tree, elem);
}

static void releaseRecurse(struct BinarySearchTreeArray * tree, int nodeNum)
{
    if (nodeNum == -1)
    {
        return;
    }
    tree->freeFunction(tree->tree[nodeNum].element);
    releaseRecurse(tree, tree->tree[nodeNum].leftChild);
    releaseRecurse(tree, tree->tree[nodeNum].rightChild);
}

void releaseBinarySearchTreeArray(struct BinarySearchTreeArray * tree)
{
    releaseRecurse(tree, tree->root);
    free(tree->tree);
}

static void printRecurse(struct BinarySearchTreeArray * tree, int node)
{
    if (node == -1)
    {
        //printf("reached the end\n");
        return;
    }
    int i;
    printRecurse(tree, tree->tree[node].leftChild);
    tree->copyFunction(&i, tree->tree[node].element, tree->elementSize);
    printf("%d(%d,%d,%d) ", i, node, tree->tree[node].leftChild, tree->tree[node].rightChild);
    printRecurse(tree, tree->tree[node].rightChild);
}

void printTree(struct BinarySearchTreeArray * tree)
{
    //printf("current size is %d\n", tree->currentSize);
    if (isEmpty(tree))
    {
        printf("empty tree\n");
    }
    else
    {
        printRecurse(tree, 0);
        printf("\n");
    }
}