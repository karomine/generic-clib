#include "DequeLink.h"
#include <stdio.h>

struct Node {
    struct Node * next;
    struct Node * previous;
    void * element;
};

static struct Node * makeNode(struct DequeLink * deque, void * element)
{
    struct Node * node = malloc(sizeof *node);
    node->element = malloc(deque->elementSize);
    deque->copyFunction(node->element, element, deque->elementSize);
    return node; 
}

int isEmptyDequeLink(struct DequeLink * deque)
{
    return deque->front == NULL;
}

void addFrontDequeLink(struct DequeLink * deque, void * element)
{
    struct Node * newFront = makeNode(deque, element);
    if (isEmptyDequeLink(deque))
    {
        deque->back = newFront;
    }
    else
    {
        deque->front->previous = newFront;
    }
    newFront->next = deque->front;
    newFront->previous = NULL;
    deque->front = newFront;
}

void addBackDequeLink(struct DequeLink * deque, void * element)
{
    //printf("adding to back\n");
    struct Node * newBack = makeNode(deque, element);
    if (isEmptyDequeLink(deque))
    {
        deque->front = newBack;
    }
    else
    {
        deque->back->next = newBack;
    }
    newBack->previous = deque->back;
    newBack->next = NULL;
    deque->back = newBack;
}

void getFrontDequeLink(struct DequeLink * deque, void * reciever)
{
    deque->copyFunction(reciever, deque->front->element, deque->elementSize);
}

void getBackDequeLink(struct DequeLink * deque, void * reciever)
{
    deque->copyFunction(reciever, deque->back->element, deque->elementSize);
}

void removeFrontDequeLink(struct DequeLink * deque)
{
    struct Node * toRemove = deque->front;
    if (deque->front == deque->back)
    {
        deque->back = NULL;
    }
    else
    {
        deque->front->next->previous = NULL;
    }
    deque->front = toRemove->next;
    deque->freeFunction(toRemove->element);
    free(toRemove);
}

void removeBackDequeLink(struct DequeLink * deque)
{
    struct Node * toRemove = deque->back;
    if (deque->front == deque->back)
    {
        deque->front = NULL;
    }
    else
    {
        deque->back->previous->next = NULL;
    }
    deque->back = toRemove->previous;
    deque->freeFunction(toRemove->element);
    free(toRemove);
}

struct DequeLink * makeDequeLink(int elementSize,
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct DequeLink * deque = malloc(sizeof *deque);
    deque->front = NULL;
    deque->back = NULL;
    deque->elementSize = elementSize;
    deque->copyFunction = copyFunction;
    deque->freeFunction = freeFunction;
    return deque;
}

