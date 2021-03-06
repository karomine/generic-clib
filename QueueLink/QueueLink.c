#include "QueueLink.h"

struct Node 
{
	void * element;
	struct Node * next;
};

static struct Node * makeNode(struct LinkQueue * queue, void * element) 
{
	struct Node * link = malloc(sizeof *link);
	link->element = malloc(queue->elementSize);
	queue->copyFunction(link->element, element, queue->elementSize);
	return link;
}

void pushLinkQueue(struct LinkQueue * queue, void * element) 
{
	struct Node * newTail = makeNode(queue, element);
	if (isEmptyLinkQueue(queue)) {
		queue->head = newTail;
	}
	else {
		queue->tail->next = newTail;
	}
	queue->tail = newTail; 
}

void popLinkQueue(struct LinkQueue * queue) 
{
	struct Node * toRemove = queue->head;
	if (queue->head == queue->tail) {
		queue->head = queue->tail = NULL;
	}
	else {
		queue->head = toRemove->next;
	}
	queue->freeFunction(toRemove->element);
	free(toRemove);
}

void frontLinkQueue(struct LinkQueue * queue, void * reciever) 
{
	queue->copy(reciever, queue->head->element, queue->elementSize);
}

int isEmptyLinkQueue(struct LinkQueue * queue) 
{
	return queue->head == NULL;
}

void releaseLinkQueue(struct LinkQueue * queue) {
    struct node * iterator = queue->head;
    struct node * toRemove;
    while (iterator) {
        toRemove = iterator;
        iterator = iterator->next;
        queue->freeFunction(toRemove->element);
        free(toRemove);
    }
    free(queue); 
}

struct LinkQueue * makeCustomLinkQueue(int elementSize,
    void *(*copyFunction)(void *, const void *, size_t),
    void(*freeFunction)(void *))
{
    struct LinkQueue * queue = malloc(sizeof *queue);
    queue->elementSize = elementSize;
    queue->copyFunction = copyFunction;
    queue->freeFunction = freeFunction; 
    queue->head = NULL;
    queue->tail = NULL;
}
