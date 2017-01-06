/*
Generic link based double ended queue implementation
Written by - Kevin Romine
language - C
*/

#ifndef _DEQUE_LINK_H_
#define _DEQUE_LINK_H_

#include <stdlib.h>

/*
** front and back managed internally
** elementSize, copyFunction, and freeFunction provided by caller in makeDequeLink
** elementSize - size of data type put on deque
** copyFunction
    ** how to copy data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
** freeFunction
    ** how to free a dynamically allocated element of the data type
    ** can just pass free it no dynamic allocation done in copyFunction
*/

struct DequeLink {
    struct Node * front;
    struct Node * back;
    int elementSize;
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
};

// return true if the deque is empty
int isEmptyDequeLink(struct DequeLink * deque);

// adds an element to the front of the deque by address
void addFrontDequeLink(struct DequeLink * deque, void * element);

// adds an element to the back of the deque by address, 
void addBackDequeLink(struct DequeLink * deque, void * element);

// copies the front of the deque to the address of reciever, caller must ensure not empty
void getFrontDequeLink(struct DequeLink * deque, void * reciever);

// copies the back of the deque to the address of reciever, caller must ensure not empty
void getBackDequeLink(struct DequeLink * deque, void * reciever);

// removes the front of the deque, caller must ensure not empty
void removeFrontDequeLink(struct DequeLink * deque);

// removes the back of the deque, caller must ensure not empty
void removeBackDequeLink(struct DequeLink * deque);

// deallocates the deque and frees the deque structure
void releaseDequeLink(struct DequeLink * deque);

// constructs a deque with properties suiting a certain data type
struct DequeLink * makeDequeLink(int elementSize,
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

#endif
