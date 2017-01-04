#include "QueueArray.h"
#include <stdio.h>

static void freeQueueArray(struct ArrayQueue * queue) {
    int iterator;
    if (queue->freeFunction) {
        for (iterator = queue->head; iterator != queue->tail; iterator = (iterator + 1) % queue->maxSize)
        {
            queue->freeFunction(queue->queue + queue->elementSize * iterator);
        }
        queue->freeFunction(queue->queue + queue->elementSize * iterator);
    }
    free(queue->queue);
}


static void copyQueueElement(struct ArrayQueue * queue, char * destination, int destinationIndex, int sourceIndex)
{
    queue->copyFunction(destination + queue->elementSize * destinationIndex,
        queue->queue + queue->elementSize * sourceIndex, queue->elementSize);
}

static void collapseQueue(struct ArrayQueue * queue)
{
    char * newQueue = malloc(queue->maxSize * queue->elementSize);
    int index, iterator;
    for (index = 0, iterator = queue->head; iterator != queue->tail; index++, iterator = (iterator + 1) % queue->maxSize)
    {
        copyQueueElement(queue, newQueue, index, iterator);
    }
    copyQueueElement(queue, newQueue, index, iterator);
    freeQueueArray(queue);
    queue->queue = newQueue;
    queue->head = 0;
    queue->tail = index;
}

static void setSize(struct ArrayQueue * queue, int size) {
    collapseQueue(queue); 
    queue->maxSize = size;
    queue->queue = realloc(queue->queue, queue->maxSize * queue->elementSize);
}

static void grow(struct ArrayQueue * queue) 
{
    setSize(queue, queue->maxSize * 2);
}

static void shrink(struct ArrayQueue * queue)
{
    setSize(queue, queue->maxSize / 2);
}

static int full(struct ArrayQueue * queue)
{
    return queue->currentSize == queue->maxSize; 
}

void push(struct ArrayQueue * queue, void * element) 
{
    if (empty(queue)) 
    {
        queue->copyFunction(queue->queue, element, queue->elementSize);
        queue->head = queue->tail = 0;
        queue->currentSize = 1; 
    }
    else
    {
        if (full(queue))
        {
            grow(queue);
        }
        queue->tail = (queue->tail + 1) % queue->maxSize;
        queue->copyFunction(queue->queue + queue->tail * queue->elementSize,
            element, queue->elementSize);
        queue->currentSize++;
    }
}

void pop(struct ArrayQueue * queue)
{
    if (queue->freeFunction) {
        queue->freeFunction(queue->queue + queue->head * queue->elementSize);
    }
    queue->head = (queue->head + 1) % queue->maxSize;
    queue->currentSize--;
    if (queue->currentSize < queue->maxSize / 4 && queue->currentSize > 8) 
    {
        shrink(queue);
    }
}

void top(struct ArrayQueue * queue, void * reciever) 
{
    queue->copyFunction(reciever, queue->queue + queue->head * queue->elementSize, queue->elementSize);
}

int empty(struct ArrayQueue * queue) 
{
    return queue->currentSize == 0;
}

void release(struct ArrayQueue * queue) {
    freeQueueArray(queue);
    free(queue);
}

struct ArrayQueue * makeArrayQueue(int elementSize, int startSize,
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct ArrayQueue * queue = malloc(sizeof *queue);
    queue->currentSize = 0;
    queue->maxSize = startSize;
    queue->elementSize = elementSize;
    queue->queue = malloc(startSize * elementSize);
    queue->copyFunction = copyFunction;
    queue->freeFunction = freeFunction;
}