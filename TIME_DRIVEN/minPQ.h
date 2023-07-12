#ifndef MIN_PQ_H_ 
#define MIN_PQ_H
#include"event.h"

typedef struct minPQ{
	Event** events;
	int capacity;
	int size;
}minPQ;

minPQ* createPriorityQueue(int initialCapacity);

void destroyPriorityQueue(minPQ* queue);

void enqueue(minPQ* queue, Event* event);

Event* dequeue(minPQ* queue);

#endif
