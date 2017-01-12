#include "BinaryTreeLink.h"

struct Node
{
    void * element;
    struct Node * leftChild;
    struct Node * rightChild;
};

static void releaseNode(struct BinarySearchTreeLink * tree, struct Node * node)
{
    tree->freeFunction(node->element);
    free(node);
}

static int isLeafNode(struct Node * node)
{
    return noLeftChild(node) && noRightChild(node);
}

static int noLeftChild(struct Node * node)
{
    return node->leftChild == NULL;
}

static int noRightChild(struct Node * node)
{
    return node->rightChild == NULL;
}

static struct Node * makeNode(struct BinarySearchTreeLink * tree, void * element)
{
    struct Node * node = malloc(sizeof *node);
    node->element = malloc(sizeof(tree->elementSize));
    tree->copyFunction(node->element, element, tree->elementSize);
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

static struct Node * addRecurse(struct BinarySearchTreeLink * tree, struct Node * node, void * element)
{
    if (node == NULL)
    {
        struct Node * newNode = makeNode(tree, element);
        return newNode;
    }
    int comparison = tree->compareFunction(element, node->element);
    if (comparison >= 0)
    {
        node->rightChild = addRecurse(tree, node->rightChild, element);
    }
    else
    {
        node->leftChild = addRecurse(tree, node->leftChild, element);
    }
    return node;
}

void addBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element)
{
    tree->root = addRecurse(tree, tree->root, element);
}

static void swap(struct Node * n1, struct Node * n2)
{
    void * temp = n1->element;
    n1->element = n2->element;
    n2->element = temp;
}

static struct Node * removeNode(struct BinarySearchTreeLink *, struct Node *);

static struct Node * searchLeft(struct BinarySearchTreeLink * tree, struct Node * node, struct Node * swapWith)
{
    while (node->leftChild)
    {
        node = node->leftChild;
    }
    swap(node, swapWith);
    return removeNode(tree, node);
}

static struct Node * searchRight(struct BinarySearchTreeLink * tree, struct Node * node, struct Node * swapWith)
{
    while (node->rightChild)
    {
        node = node->rightChild;
    }
    swap(node, swapWith);
    return removeNode(tree, node);
}

static struct Node * removeNode(struct BinarySearchTreeLink * tree, struct Node * node)
{
    if (isLeafNode(node))
    {
        releaseNode(tree, node);
        return NULL;
    }
    else if (noLeftChild(node))
    {
        releaseNode(tree, node);
        return node->rightChild;
    }
    else if (noRightChild(node))
    {
        releaseNode(tree, node);
        return node->leftChild;
    }
    else if (node->leftChild == NULL)
    {
        node->rightChild = searchLeft(tree, node->rightChild, node);
    }
    else
    {
        node->rightChild = searchRight(tree, node->leftChild, node);
    }
    return node;
}

static struct Node * removeRecurse(struct BinarySearchTreeLink * tree, struct Node * node, void * element)
{
    if (node == NULL)
    {
        return NULL;
    }
    int comparison = tree->compareFunction(element, node->element);
    if (comparison == 0)
    {
        return removeNode(tree, node);
    }
    else if (comparison > 0)
    {
        node->rightChild = removeRecurse(tree, node->rightChild, element);
    }
    else
    {
        node->leftChild = removeRecurse(tree, node->leftChild, element);
    }
    return node;
}

void removeBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element)
{
    tree->root = removeRecurse(tree, tree->root, element);
}

static int containsRecurse(struct BinarySearchTreeLink * tree, struct Node * node, void * element)
{
    if (node == NULL)
    {
        return 0;
    }
    int comparison = tree->compareFunction(element, node->element);
    if (comparison == 0)
    {
        return 1;
    }
    else if (comparison > 0)
    {
        return containsRecurse(tree, node->rightChild, element);
    }
    else
    {
        return containsRecurse(tree, node->leftChild, element);
    }
}

int containsBinarySearchTreeLink(struct BinarySearchTreeLink * tree, void * element)
{
    return containsRecurse(tree, tree->root, element);
}

struct BinarySearchTreeLink * makeBinarySearchTreeLink(int elementSize,
    int(*compareFunction)(const void *, const void *),
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct BinarySearchTreeLink * tree = malloc(sizeof *tree);
    tree->elementSize = elementSize; 
    tree->compareFunction = compareFunction;
    tree->copyFunction = copyFunction;
    tree->freeFunction = freeFunction;
    tree->root = NULL;
    return tree;
}