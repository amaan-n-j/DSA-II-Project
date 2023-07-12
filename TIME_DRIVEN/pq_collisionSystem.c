#include<stdlib.h>
#include<stdio.h>
#include"pq_collisionSystem.h"
#include<math.h>
#include<time.h>
#include<limits.h>
//#include"particle.h"
//#include"event.h"
//#include"minPQ.h"

void predict(Particle* p, minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys){
        if(p == NULL) return;
        double dt;
	Event* event;
        for (int i = 0; i < sys->particleCount; i++){
                dt = timeToHit(p, &(sys->particleArray[i]));
                if(dt != INT_MAX && dt == dt){//dt == dt check for nan value
                        event = newEvent(pq_sys->t+dt, p, &(sys->particleArray[i]));
                        enqueue(pq, event);
                }
        }
        dt = timeToHitVerticalWall(p);
        if(dt != INT_MAX && dt == dt){
                event = newEvent(pq_sys->t+dt, p, NULL);
                enqueue(pq, event);
        }
        dt = timeToHitHorizontalWall(p);
        if(dt != INT_MAX && dt == dt){
                event = newEvent(pq_sys->t+dt, NULL, p);
       		enqueue(pq, event);
        }
}

void redraw(minPQ* pq, pq_CollisionSystem* pq_sys){
	//printf("Redraw");
        Event* event = newEvent(pq_sys->t + 1, NULL, NULL); //1 is the minimum time after which redraw will be called
        enqueue(pq, event);
}

void fillPQ(minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys){
//	printf("%d %d", pq->capacity, sys->particleCount);
        for(int i = 0;i < sys->particleCount;i++){
		predict(&(sys->particleArray[i]), pq, sys, pq_sys); //fill PQ at the first render with all possible collisions
	}
	Event* event = newEvent(0, NULL, NULL); //initial redraw event
        enqueue(pq, event); //initial redraw event
}

void updatePQ(minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys){
        
	while(pq->size != 0){
		Event* event = dequeue(pq);
        	//if(!isValid(event)) return;
		//printf("%f\n", event->time);
		while(!isValid(event)){
			//printf("Invalid");
			free(event);
			event = dequeue(pq);
		}
        	Particle* particle1 = event->particle1;
        	Particle* particle2 = event->particle2;

        	for(int i = 0;i < sys->particleCount;i++){
        	        move(&(sys->particleArray[i]), event->time - pq_sys->t);
        	}
		pq_sys->t = event->time;
		//printf("%f ", pq_sys->t);
		if(particle1 != NULL && particle2 != NULL) bounceOff(particle1, particle2);
		else if(particle1 != NULL && particle2 == NULL) bounceOffVerticalWall(particle1);
		else if (particle1 == NULL && particle2 != NULL) bounceOffHorizontalWall(particle2);
		else if (particle1 == NULL && particle2 == NULL) {
			redraw(pq, pq_sys);
			free(event);
			return;
		}
		predict(particle1, pq, sys, pq_sys);
		predict(particle2, pq, sys, pq_sys);
		//printf("%d ", pq->size);
		free(event);
	}
}


//int main(){
//	particleSystem* ps;
//	createRandomSystem(&ps, 200);
//	minPQ* pq = createPriorityQueue();
//	fillPQ()
//	
//}
