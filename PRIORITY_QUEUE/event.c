#include"event.h"
#include"particle.h"
#include<stdio.h>
#include<stdlib.h>

Event* newEvent(double time, Particle* particle1, Particle* particle2){
	Event* e = malloc(sizeof(Event));
        if(!e){
        	printf("Error occured while creating a new event\n");
                return NULL;
        }
        e->time = time;
        //e->type = type;
        e->particle1 = particle1;
        e->particle2 = particle2;
	if(particle1 != NULL){
		e->countA = particle1->collisions;
	}else{
		e->countA = 0;
	}
	if(particle2 != NULL){
        	e->countB = particle2->collisions;	
	}else{
		e->countB = 0;
	}
	return e;
}

int compare(Event* event1, Event* event2){
	if(event1->time == event2->time) return 0;
	if(event1->time > event2->time) return 1;
	return -1;
}

int isValid(Event* event){
	if((event->particle1 != NULL && (event->particle1->collisions != event->countA)) || (event->particle2 != NULL && (event->particle2->collisions != event->countB))){
		//if collision count of a particle involved in an event changes then it implies some other event intervened and changed the collision count of the particle
		return 0;
	}
	return 1;
}

//int main(){
//	Color c1;
//	c1.r = 1;
//	c1.g = 1;
//	c1.g = 0;
//	Particle* p1 = newParticle(0, 0, 0.2, 0.1, 0.1, 1, c1);
//	Particle* p2 = newParticle(0.3,-0.1, 0.1, -0.1, 0.1, 1, c1);
//	Event* e1 = newEvent(10, p1, p2);
//	Event* e2 = newEvent(20, p1, p2);
//	p1->collisions++;
//	printf("%d\n",isValid(e2));
//}       

