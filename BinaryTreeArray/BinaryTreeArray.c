#include "BinaryTreeArray.h"

struct Node {
    void * element;
    int leftChild;
    int rightChild;
    int parent;
};

static int searchRight(struct BinarySearchTreeArray * tree, int nodeNum)
{
    //find position in tree once down right child, then as far left as possible and return it
    nodeNum = tree->tree[nodeNum].rightChild;
    while (tree->tree[nodeNum].leftChild != -1)
    {
        nodeNum = tree->tree[nodeNum].leftChild;
    }
    return nodeNum;
}

static int searchLeft(struct BinarySearchTreeArray * tree, int nodeNum)
{
    //find position in tree once down left child, then as far right possible and return it
    nodeNum = tree->tree[nodeNum].leftChild;
    while (tree->tree[nodeNum].rightChild != -1)
    {
        nodeNum = tree->tree[nodeNum].rightChild;
    }
    return nodeNum;
}

static void resetParent(struct BinarySearchTreeArray * tree, int oldNum, int newNum)
{
    struct Node * parent = &tree->tree[tree->tree[oldNum].parent];
    //get pointer to parent of node at oldNum
    if (parent->leftChild == oldNum)
    {
        parent->leftChild = newNum;
    }
    else
    {
        parent->rightChild = newNum;
    }
    //set whatever child in the parent was referring to the old num to now refer to the new num
}

static void reorganize(struct BinarySearchTreeArray * tree, int nodeNum)
{
    tree->freeFunction(tree->tree[nodeNum].element);
    //free element of deleted node
    resetParent(tree, nodeNum, -1);
    //set parent to now point to -1 instead of deleted node
    --tree->currentSize;
    //decrease tree size
    if (nodeNum != tree->currentSize)
    {
        //move node at end of array to newly freed location
        tree->tree[nodeNum] = tree->tree[tree->currentSize];
        //point parent node to new location
        resetParent(tree, tree->currentSize, nodeNum);
    }
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
    setSize(tree, tree->maxSize * 2);
}

static int isLeafNode(struct BinarySearchTreeArray * tree, int nodeNum)
{
    return tree->tree[nodeNum].leftChild == -1 && tree->tree[nodeNum].rightChild == -1;
}

struct BinarySearchTreeArray * makeBinarySearchTreeArray(int elementSize, int startSize,
		     int (*comparisonFunction)(const void *,const void *, size_t),
		     void * (*copyFunction)(void *, const void *, size_t),
		     void (*freeFunction)(void *))
{
    struct BinarySearchTreeArray * tree = malloc(sizeof *tree);
    tree->tree = malloc(startSize * sizeof *tree->tree); 
    tree->elementSize = elementSize;
    tree->root = -1;
    tree->comparisonFunction = comparisonFunction;
    tree->copyFunction = copyFunction;
    tree->freeFunction = freeFunction;
    tree->currentSize = 0;
    tree->maxSize = startSize; 
    return tree;
}

static void swap(struct BinarySearchTreeArray * tree, int pos1, int pos2)
{
    //swap the elements at two given nodes in the tree
    void * temp = tree->tree[pos1].element;
    tree->tree[pos1].element = tree->tree[pos2].element;
    tree->tree[pos2].element = temp;
}

static void removeNode(struct BinarySearchTreeArray * tree, int nodeNum) {
    int swapPosition;
    if (isLeafNode(tree, nodeNum))
    {
        //got node to delete at leaf, do final reorganization
        reorganize(tree, nodeNum);
        return;
    }
    else if (tree->tree[nodeNum].leftChild == -1)
    {
        //get node position at right once, then left as far as possible
        swapPosition = searchRight(tree, nodeNum);
    }
    else
    {
        //get node position at left once, then right as far as possible
        swapPosition = searchLeft(tree, nodeNum);
    }
    //swap the current node with next position
    swap(tree, nodeNum, swapPosition);
    //keep swapping until at leaf node
    removeNode(tree, swapPosition);
}

static void removeRecurse(int nodeNum, struct BinarySearchTreeArray * tree, void * element) 
{
    if (nodeNum == -1)
    {
        return;
    }
    int comparison = tree->comparisonFunction(element, tree->tree[nodeNum].element, tree->elementSize);
    if (comparison == 0) 
    {
        removeNode(tree, nodeNum);
        return;
    }
    else if (comparison > 0) 
    {
        return removeRecurse(tree->tree[nodeNum].rightChild, tree, element);
    }
    else
    {
        return removeRecurse(tree->tree[nodeNum].leftChild, tree, element);
    }
}

void removeBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element) 
{
    return removeRecurse(tree->root, tree, element); 
}

static void addRecurse(int * nodeNum, struct BinarySearchTreeArray * tree, void * element, int parent)
{
    if (*nodeNum == -1) 
    {
        //found node with no child where new node should be
        if (tree->currentSize == tree->maxSize) 
        {
            //grow the size of tree if necessary
            grow(tree);
        }
        *nodeNum = tree->currentSize++;
        //set parent to point to end of array where new node is
        tree->tree[*nodeNum].element = malloc(tree->elementSize);
        tree->copyFunction(tree->tree[*nodeNum].element, element, tree->elementSize);
        tree->tree[*nodeNum].leftChild = -1;
        tree->tree[*nodeNum].rightChild = -1;
        tree->tree[*nodeNum].parent = parent;
        //fill in info for new node
    }
    else 
    {
        int comparison = tree->comparisonFunction(element, tree->tree[*nodeNum].element, tree->elementSize);
        //keep searching down left or right child based on comparison
        if (comparison >= 0) 
        {
            addRecurse(&tree->tree[*nodeNum].rightChild, tree, element, *nodeNum);
        }
        else 
        {
            addRecurse(&tree->tree[*nodeNum].leftChild, tree, element, *nodeNum);
        }
    }
}

void addBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * element)
{
    addRecurse(&tree->root, tree, element, -1);
}

static int containsRecurse(int * nodeNum, struct BinarySearchTreeArray * tree, void * elem)
{
    if (*nodeNum == -1)
    {
        //reached end of tree without finding element
        return 0;
    }
    else
    {
        int comparison = tree->comparisonFunction(elem, tree->tree[*nodeNum].element, tree->elementSize);
        if (comparison == 0)
        {
            //found element return success
            return 1;
        }
        else if (comparison > 0)
        {
            //search right child
            return containsRecurse(&tree->tree[*nodeNum].rightChild, tree, elem);
        }
        else
        {
            //search left child
            return containsRecurse(&tree->tree[*nodeNum].leftChild, tree, elem);
        }
    }
}

int containsBinarySearchTreeArray(struct BinarySearchTreeArray * tree, void * elem) 
{
  return containsRecurse(&tree->root, tree, elem);
}

static void releaseRecurse(struct BinarySearchTreeArray * tree, int * nodeNum) 
{
    if (*nodeNum == -1) 
    {
        return;
    }
    tree->freeFunction(tree->tree[*nodeNum].element);
    releaseRecurse(tree, &tree->tree[*nodeNum].leftChild);
    releaseRecurse(tree, &tree->tree[*nodeNum].rightChild);
}
  
void releaseBinarySearchTreeArray(struct BinarySearchTreeArray * tree)
{
    releaseRecurse(tree, &tree->root);
    free(tree->tree);
}

