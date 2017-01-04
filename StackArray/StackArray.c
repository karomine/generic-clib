#include "StackArray.h"

static void grow(struct ArrayStack * stack) 
{
    stack->maxSize *= 2;
    stack->stack = realloc(stack->stack, stack->maxSize * stack->elementSize);
}

static void shrink(struct ArrayStack * stack) {
    stack->maxSize /= 2;
    stack->stack = realloc(stack->stack, stack->maxSize * stack->elementSize);
}

static int full(struct ArrayStack * stack) 
{
    return stack->currentSize == stack->maxSize;
}

void push(struct ArrayStack * stack, void * element) 
{
    if (full(stack)) {
        grow(stack);
    }
    stack->copyFunction(stack->stack + (stack->currentSize++ * stack->elementSize),
        element, stack->elementSize);
}

void pop(struct ArrayStack * stack) 
{
    if (stack->freeFunction) {
        stack->freeFunction(stack->stack + stack->elementSize * (stack->currentSize - 1));
    }
    stack->currentSize--;
    if (stack->currentSize < stack->maxSize / 4) {
        shrink(stack); 
    }
}

void top(struct ArrayStack * stack, void * recieve) 
{
    stack->copyFunction(recieve, stack->stack + ((stack->currentSize - 1) * stack->elementSize),
        stack->elementSize);
}

int empty(struct ArrayStack * stack) 
{
    return stack->currentSize == 0;
}

void release(struct ArrayStack * stack) 
{
    int i;
    if (stack->freeFunction) {
        for (i = 0; i < stack->currentSize; i++) {
            stack->freeFunction(stack->stack + (i + stack->elementSize));
        }
    }
    free(stack->stack);
    free(stack); 
}

struct ArrayStack * makeArrayStack(int elementSize,
    int startSize, void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct ArrayStack * stack = malloc(sizeof *stack);
    stack->stack = malloc(elementSize * startSize);
    stack->currentSize = 0;
    stack->maxSize = startSize;
    stack->elementSize = elementSize;
    stack->copyFunction = copyFunction;
    stack->freeFunction = freeFunction;
    return stack; 
}
