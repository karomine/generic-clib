/* 
Generic array based queue implementation
Written by - Kevin Romine
language - C
*/

#ifndef _ARRAY_QUEUE_H_
#define _ARRAY_QUEUE_H_

#include <stdlib.h>

/*
** currentSize, maxSize, head, tail, and queue are managed internally
** elementSize, startSize, copyFunction, and freeFuncton provided by caller in makeQueueStack
** elementSize - size of data type put on queue
** startSize - starting size of queue 
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
** freeFunction
    ** how to release any dynamically allocated memory from copy function
    ** pass NULL if dealloctaion not needed
*/

struct ArrayQueue {
    int currentSize;
    int maxSize;
    int elementSize;
    int head;
    int tail;
    char * queue;
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
};

// add element to queue, pass element to add by address
void pushArrayQueue(struct ArrayQueue * queue, void * element);

// removes front of queue, caller responsible for ensuring queue is not empty (empty function)
void popArrayQueue(struct ArrayQueue * queue);

// grabs front element from queue, copys to location pointed by reciever, caller responsible for ensuring not empty
void topArrayQueue(struct ArrayQueue * queue, void * reciever);

// return true for an empty queue
int isEmptyArrayQueue(struct ArrayQueue * queue);

// free all dynamic memory in queue and free structure 
void releaseArrayQueue(struct ArrayQueue * queue);

// creates a queue with properties suiting a certain data type
struct ArrayQueue * makeArrayQueue(int elementSize, int startSize,
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

#endif
