#include "StackLink.h"

struct Node 
{
    void * element;
    struct Node * next;
};

static struct Node * makeStackNode(struct LinkStack * stack, void * element) 
{
    struct Node * node = malloc(sizeof *node);
    node->element = malloc(stack->elementSize);
    stack->copyFunction(node->element, element, stack->elementSize);
    return node;
}

void push(struct LinkStack * stack, void * element) 
{
    struct Node * top = makeStackNode(stack, element);
    top->next = stack->top;
    stack->top = top;
}

void top(struct LinkStack * stack, void * reciever) 
{
    stack->copyFunction(reciever, stack->top->element, stack->elementSize);
}

void pop(struct LinkStack * stack) 
{
    struct Node * toRemove = stack->top;
    stack->top = toRemove->next;
    stack->freeFunction(toRemove->element);
    free(toRemove);
}

int empty(struct LinkStack * stack) 
{
    return stack->top == NULL;
}

void release(struct LinkStack * stack) 
{
    struct Node * iterator = stack->top;
    struct Node * toRemove; 
    while (iterator) {
        toRemove = iterator;
        iterator = iterator->next;
        stack->freeFunction(toRemove->element);
        free(toRemove);
    }
    free(stack);
}

struct LinkStack * makeLinkStack(int elementSize, 
    void *(*copyFunction)(void *, const void *, size_t), 
    void(*freeFunction)(void *)) 
{
    struct LinkStack * stack = malloc(sizeof *stack);
    stack->top = NULL;
    stack->elementSize = elementSize;
    stack->copyFunction = copyFunction;
    stack->freeFunction = freeFunction;
    return stack;
}