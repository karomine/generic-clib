#include "StackArray.h"

static void setSize(struct ArrayStack * stack, int size)
{
    stack->maxSize = size;
    stack->stack = realloc(stack->stack, stack->maxSize * stack->elementSize);
}

static void grow(struct ArrayStack * stack) 
{
    setSize(stack, stack->maxSize * 2);
}

static void shrink(struct ArrayStack * stack) 
{
    setSize(stack, stack->maxSize / 2);
}

static int isFull(struct ArrayStack * stack) 
{
    return stack->currentSize == stack->maxSize;
}

void pushArrayStack(struct ArrayStack * stack, void * element) 
{
    if (isFull(stack)) {
        grow(stack);
    }
    stack->copyFunction(stack->stack + (stack->currentSize++ * stack->elementSize),
        element, stack->elementSize);
}

void popArrayStack(struct ArrayStack * stack) 
{
    if (stack->freeFunction) {
        stack->freeFunction(stack->stack + stack->elementSize * (stack->currentSize - 1));
    }
    stack->currentSize--;
    if (stack->currentSize < stack->maxSize / 4) {
        shrink(stack); 
    }
}

void topArrayStack(struct ArrayStack * stack, void * recieve) 
{
    stack->copyFunction(recieve, stack->stack + ((stack->currentSize - 1) * stack->elementSize),
        stack->elementSize);
}

int isEmptyArrayStack(struct ArrayStack * stack) 
{
    return stack->currentSize == 0;
}

void releaseArrayStack(struct ArrayStack * stack) 
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
