/*
Generic link list based queue implementation
Written by - Kevin Romine
language - C 
*/

#ifndef _LINK_QUEUE_
#define _LINK_QUEUE_

#include "stdlib.h"
#include <stddef.h>


/*
** head, tail are used internally
** elementSize, copyFunction, and freeFuncton provided by caller in makeLinkQueue
** elementSize - size of data type put on queue
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
freeFunction
    ** how to release any dynamically allocated memory from copy function
    ** pass NULL if dealloctaion not needed
*/
struct LinkQueue 
{
	struct Node * head;
	struct Node * tail;
	int elementSize;
	void *(*copyFunction)(void *, const void *, size_t);
	void(*freeFunction)(void *);
};

// adds element on back of queue, element passed by address
void push(struct LinkQueue * queue, void * element);

// removes element at the front of the queue, caller responsible for ensuring queue is not empty (empty function)
void pop(struct LinkQueue * queue);

// copys element at front of queue to location pointed by reciever
void front(struct LinkQueue * queue, void * reciever);

// returns true of queue is empty
int empty(struct LinkQueue * queue);

// deallocates all memory allocated to queue and frees queue sructure
void release(struct LinkQueue * queue);

// creates queue with properties suiting a certain data type
struct LinkQueue * makeCustomLinkQueue(int elementSize, 
    void * (*copyFunction)(void *, const void *, size_t), 
    void(*freeFunction)(void *));

#endif 

