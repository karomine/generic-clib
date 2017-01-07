/*
Generic array based priority queue implementation
Written by - Kevin Romine
language - C
*/

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdlib.h>

/*
** queue, currentSize and maxSize used internally
** elementSize, compareFunction, copyFunction, and freeFunction provided by caller in makePriorityQueue
** elementSize - size of data type put on queue
** startSize - starting size of priority queue
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
** freeFunction
    ** how to release any dynamically allocated memory from copy function
    ** pass free if deallocation not needed
** compareFunction
    ** function to compare the ordering of two elements
    ** return -1 arg1 < arg2, 0 for arg1 == arg2, 1 for arg1 > arg2
*/

struct PriorityQueue {
    void ** queue;
    int elementSize;
    int currentSize;
    int maxSize; 
    int(*compareFunction)(const void *, const void *);
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
};

// get element with max priority, copied to location pointed to by reciever
void getMaxPriorityQueue(struct PriorityQueue * queue, void * reciever);

// removes the item with max priority from the queue
void removeMaxPriorityQueue(struct PriorityQueue * queue);

// adds an element to the priority queue, passed by address
void insertPriorityQueue(struct PriorityQueue * queue, void * element);

// deallocates internal memory and frees the queue structure
void releasePriorityQueue(struct PriorityQueue * queue);

// returns true if the queue is empty
int isEmptyPriorityQueue(struct PriorityQueue * queue); 

// creates a priority queue with properties suiting a certain data type
struct PriorityQueue * makePriorityQueue(int elementSize, int startSize,
    int(*compareFunction)(const void *, const void *),
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

#endif