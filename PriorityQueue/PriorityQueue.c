#include "PriorityQueue.h"
#include <stdio.h>

static int isLeafNode(struct PriorityQueue * queue, int node)
{
    return node * 2 + 1 >= queue->currentSize;
}

static void setSize(struct PriorityQueue * queue, int size)
{
    queue->maxSize = size;
    queue->queue = realloc(queue->queue, queue->maxSize * sizeof(void *));
}

static void grow(struct PriorityQueue * queue)
{
    setSize(queue, queue->maxSize * 2);
}

static void shrink(struct PriorityQueue * queue)
{
    setSize(queue, queue->maxSize / 2);
}

static void swap(struct PriorityQueue * queue, int pos1, int pos2)
{
    void * temp = queue->queue[pos1];
    queue->queue[pos1] = queue->queue[pos2];
    queue->queue[pos2] = temp;
}

void getMaxPriorityQueue(struct PriorityQueue * queue, void * reciever)
{
    queue->copyFunction(reciever, queue->queue[0], queue->elementSize);
}

void removeMaxPriorityQueue(struct PriorityQueue * queue)
{
    void ** elements = queue->queue;
    queue->currentSize--;
    swap(queue, 0, queue->currentSize);
    queue->freeFunction(elements[queue->currentSize]);
    int index = 0;
    int leftComparison, rightComparison, childComparison;
    while (1)
    {
        if (isLeafNode(queue, index))
        {
            break;
        }
        leftComparison = queue->compareFunction(elements[index], elements[index * 2 + 1]);
        if (index * 2 + 2 == queue->currentSize)
        {
            if (leftComparison < 0)
            {
                swap(queue, index, index * 2 + 2);
            }
            break;
        }
        rightComparison = queue->compareFunction(elements[index], elements[index * 2 + 2]);
        if (leftComparison > 0 && rightComparison > 0)
        {
            break; 
        }
        childComparison = queue->compareFunction(elements[index * 2 + 1], elements[index * 2 + 2]);
        if (childComparison > 0)
        {
            swap(queue, index, index * 2 + 1);
            index = index * 2 + 1;
        }
        else
        {
            swap(queue, index, index * 2 + 2);
            index = index * 2 + 2;
        }
    }
    if (queue->currentSize < queue->maxSize / 4)
    {
        shrink(queue);
    }
}

void insertPriorityQueue(struct PriorityQueue * queue, void * element)
{
    if (queue->currentSize == queue->maxSize)
    {
        grow(queue); 
    }
    int index = queue->currentSize++;
    queue->queue[index] = malloc(queue->elementSize);
    queue->copyFunction(queue->queue[index], element, queue->elementSize);
    while (index != 0 && queue->compareFunction(queue->queue[index], queue->queue[index / 2]) > 0)
    {
        swap(queue, index, (index - 1) / 2);
        index = (index - 1) / 2; 
    }
}

void releasePriorityQueue(struct PriorityQueue * queue)
{
    int i, n;
    for (i = 0, n = queue->currentSize; i < n; i++)
    {
        queue->freeFunction(queue->queue[i]);
    }
    free(queue);
}

int isEmptyPriorityQueue(struct PriorityQueue * queue)
{
    return queue->currentSize == 0;
}

struct PriorityQueue * makePriorityQueue(int elementSize, int startSize,
    int(*compareFunction)(const void *, const void *),
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct PriorityQueue * queue = malloc(sizeof *queue);
    queue->currentSize = 0;
    queue->maxSize = startSize;
    queue->elementSize = elementSize;
    queue->compareFunction = compareFunction;
    queue->copyFunction = copyFunction;
    queue->freeFunction = freeFunction;
    queue->queue = malloc(startSize * sizeof(void *));
}
