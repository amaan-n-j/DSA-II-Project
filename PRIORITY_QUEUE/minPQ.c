#include <stdio.h>
#include <stdlib.h>
#include"event.h"
#include"minPQ.h"

minPQ* createPriorityQueue(int initialCapacity) {
    	minPQ* queue = (minPQ*)malloc(sizeof(minPQ));
    	queue->events = (Event**)malloc(initialCapacity * sizeof(Event*));
    	queue->capacity = initialCapacity;
   	queue->size = 0;
	return queue;
}

void destroyPriorityQueue(minPQ* queue) {
    	free(queue->events);
    	free(queue);
}

void swap(Event** event1, Event** event2) {
    	Event* temp = *event1;
    	*event1 = *event2;
    	*event2 = temp;
}

void enqueue(minPQ* queue, Event* event) {
	if (queue->size == queue->capacity) {
		// Double the capacity of the queue
		int newCapacity = queue->capacity * (3.0/2.0);
		Event** newEvents = (Event**)realloc(queue->events, newCapacity * sizeof(Event*));
		
		if (newEvents == NULL) {
		    	printf("Failed to resize the priority queue.\n");
		    	return;
		}
	
		queue->events = newEvents;
		queue->capacity = newCapacity;
	}
	//printf("%f \n", event->time);
	int currentIndex = queue->size;
	int parentIndex = (currentIndex - 1) / 2;
	
	// Percolate up
	while (currentIndex > 0 && event->time < queue->events[parentIndex]->time) {
		swap(&queue->events[currentIndex], &queue->events[parentIndex]);
	    	currentIndex = parentIndex;
	    	parentIndex = (currentIndex - 1) / 2;
	}
	
	queue->events[currentIndex] = event;
	queue->size++;
}

/*void enqueue(minPQ* queue, Event* event) {
    	if (queue->size == queue->capacity) {
		if(queue->events[queue->size - 1]->time < event->time){
			free(event);
			return;
		}	
    	    	// Remove the element at the last index
    	    	Event* removedEvent = queue->events[queue->size - 1];
    	    	free(removedEvent);

    	    	// Add the new event at the last index
    	    	queue->events[queue->size - 1] = event;
    	}

    	// Rest of the code remains the same
    	int currentIndex = queue->size;
    	int parentIndex = (currentIndex - 1) / 2;

    	// Percolate up
    	while (currentIndex > 0 && event->time < queue->events[parentIndex]->time) {
    	    	swap(&queue->events[currentIndex], &queue->events[parentIndex]);
    	    	currentIndex = parentIndex;
    	    	parentIndex = (currentIndex - 1) / 2;
    	}

    	queue->events[currentIndex] = event;
    	queue->size++;
}*/

Event* dequeue(minPQ* queue) {
	if (queue->size == 0) {
       		printf("Priority queue is empty. Cannot dequeue.\n");
          	return NULL;
	}

	Event* dequeuedEvent = queue->events[0];
	queue->size--;
	
	// Move the last event to the root
	queue->events[0] = queue->events[queue->size];
	
	int currentIndex = 0;
	int leftChildIndex = 2 * currentIndex + 1;
	int rightChildIndex = 2 * currentIndex + 2;
	
	// Percolate down
	while (leftChildIndex < queue->size) {
		int minIndex = currentIndex;
		
		if (queue->events[leftChildIndex]->time < queue->events[minIndex]->time)
			minIndex = leftChildIndex;
		
		if (rightChildIndex < queue->size && queue->events[rightChildIndex]->time < queue->events[minIndex]->time)
		    	minIndex = rightChildIndex;
		
		if (minIndex == currentIndex)
		    	break;
		
		swap(&queue->events[currentIndex], &queue->events[minIndex]);
		
		currentIndex = minIndex;
		leftChildIndex = 2 * currentIndex + 1;
		rightChildIndex = 2 * currentIndex + 2;
	}
	
	return dequeuedEvent;
}

//int main() {
//	minPQ* eventQueue = createPriorityQueue(10);
//	Color c1;
//     	c1.r = 1;
//       	c1.g = 1;
//       	c1.g = 0;
//
//	
//	Particle* p1 = newParticle(0, 0, 0.2, 0.1, 0.1, 1, c1);
//	Particle* p2 = newParticle(0.3,-0.1, 0.1, -0.1, 0.1, 1, c1);
//	// Create some sample events
//	Event* event1 = newEvent(0.1,p1,p2);
//	enqueue(eventQueue, event1);
//	
//	Event* event2 = newEvent(0.2,NULL,NULL);
//	enqueue(eventQueue, event2);
//	
//	Event* event3 = newEvent(0.4,NULL,NULL);
//	enqueue(eventQueue, event3);
//	
//	Event* event4 = newEvent(0.12,NULL,NULL);
//	enqueue(eventQueue, event4);
//	
//	Event* event5 = newEvent(0.0,NULL,NULL);
//	enqueue(eventQueue, event5);
//	
//	Event* event6 = newEvent(0.9,NULL,NULL);
//	enqueue(eventQueue, event6);
//	
//	while (eventQueue->size > 0) {
//	    	Event* dequeuedEvent = dequeue(eventQueue);
//	    	// Process the event
//	    	printf("Event time: %f\n", dequeuedEvent->time);
//	    	// Free memory allocated for the event
//	    	free(dequeuedEvent);
//	}
//	
//	destroyPriorityQueue(eventQueue);
//	
//	return 0;
//}

