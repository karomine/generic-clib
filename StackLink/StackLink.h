/*
Generic linked list stack implementation
Written by - Kevin Romine
language - C
*/

#ifndef _LINK_STACK_H_
#define _LINK_STACK_H_

#include <stdlib.h>

/*
** top is used internally
** elementSize, copyFunction, and freeFunction passed by caller to makeLinkStack
** elementSize - size of data type put on stack
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
** freeFunction
    ** how to release a dynamically allocated type put on the stack
    ** pass function that frees memory allocated in copyFunction and then frees the memory location passed in
    ** just pass free if no internal deallocation necessary
*/
struct LinkStack 
{
    struct Node * top;
    int elementSize;
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *); 
};

// add element to stack, pass element to add by address
void push(struct LinkStack * stack, void * element);

//removes top of the stack, caller responsible for ensuring stack is not empty (use empty function)
void pop(struct LinkStack * stack);

//grabs top element from stack, copys to location pointed by reciever
void top(struct LinkStack * stack, void * reciever);

//returns true for an empty stack
int empty(struct LinkStack * stack);

//deallocates all memory allocated to stack and frees the stack structure
void release(struct LinkStack * stack);

//creates an stack with properties suiting a certain data type
struct LinkStack * makeLinkStack(int elementSize,
	void *(*copyFunction)(void *, const void *, size_t),
	void(*freeFunction)(void *));

#endif