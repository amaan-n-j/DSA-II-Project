#include "particle.h"
#include<stdio.h>
#include<stdlib.h>

Particle* newParticle(double x, double y, double vx, double vy, double radius, double mass, Color color){
	Particle* p = malloc(sizeof(Particle));
	if(!p){
		printf("Error occured while creating a new particle\n");
		return NULL;
	}
	p->x = x;
	p->y = y;
	p->vx = vx;
	p->vy = vy;
	p->radius = radius;
	p->mass = mass;
	p->color = color;
	p->collisions = 0;
	return p;
}

void move(Particle* p, double dt){
	p->x += p->vx * dt;
	p->y += p->vy * dt;
}
