#ifndef EVENT_H_
#define EVENT_H_

#include "particle.h"

typedef struct Event{
	double time; //time for event to occur
	Particle *particle1, *particle2; //pointers to particles involved in the event
	int countA, countB; //collision count of particles at the time of event creation, will be used for checking validity of the event at the time of collision
}Event;

Event* newEvent(double time, Particle* particle1, Particle* particle2);

int compare(Event* event1, Event* event2);

int isValid(Event* event);

#endif
