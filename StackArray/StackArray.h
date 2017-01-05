/*
Generic array based stack implementation
Written by - Kevin Romine
language - C
*/

#ifndef _ARRAY_STACK_H_// 
#define _ARRAY_STACK_H_

#include <stdlib.h>

/*
** currentSize, maxSize, and stack are used internally
** elementSize, copyFunction, and freeFuncton provided by caller in makeArrayStack
** elementSize - size of data type put on stack
** copyFunction
    ** how to copy the data type from one memory location to another
    ** signature matches memcpy to allow simple/shallow copies by passing it in
** freeFunction
    ** how to release any dynamically allocated memory from copy function
    ** pass NULL if dealloctaion not needed
*/
struct ArrayStack {
    int currentSize;    
    int maxSize;
    int elementSize;
    char * stack;
    void *(*copyFunction)(void *, const void *, size_t);
    void(*freeFunction)(void *);
};

// add element to stack, pass element to add by address
void pushArrayStack(struct ArrayStack * stack, void * element);

// removes top of the stack, caller responsible for ensuring stack is not empty (use empty function)
void popArrayStack(struct ArrayStack * stack);

// grabs top element from stack, copys to location pointed by reciever
void topArrayStack(struct ArrayStack * stack, void * reciever);

// returns true for an empty stack
int isEmptyArrayStack(struct ArrayStack * stack);

// deallocates all memory allocated to stack and frees the stack structure
void releaseArrayStack(struct ArrayStack * stack);

// creates an stack with properties suiting a certain data type
struct ArrayStack * makeArrayStack(int elementSize, int startSize, 
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *));

#endif