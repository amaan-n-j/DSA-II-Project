#ifndef PQ_COL_SYS
#define PQ_COL_SYS

#include"particle.h"
#include"minPQ.h"
#include"randomParticleGenerator.h"
#include"event.h"

typedef struct pq_CollisionSystem{
	minPQ* pq;
	particleSystem* sys;
	double t;
}pq_CollisionSystem;

void predict(Particle* p, minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys);

void redraw(minPQ* pq, pq_CollisionSystem* pq_sys);

void fillPQ(minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys);

void updatePQ(minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys);

#endif
