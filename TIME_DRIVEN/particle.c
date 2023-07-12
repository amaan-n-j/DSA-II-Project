#include"particle.h"
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

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

double timeToHit(Particle* this, Particle* that){
	if(this == that) return INT_MAX;
	double dx = that->x - this->x;
	double dy = that->y - this->y;
	double dvx = that->vx - this->vx;
	double dvy = that->vy - this->vy;
	double dvdr = dx*dvx + dy*dvy;
	if(dvdr > 0) return INT_MAX;
	double dvdv = dvx*dvx + dvy*dvy;
	if(dvdv == 0) return INT_MAX;
	double drdr = dx*dx + dy*dy;
	double sigma = this->radius + that->radius;
	double d = (dvdr*dvdr) - dvdv * (drdr - sigma*sigma);
	if(d < 0) return INT_MAX;
	return -(dvdr + sqrt(d))/dvdv;
}

double timeToHitVerticalWall(Particle* this){
	double time;
	if(this->vx > 0){
		time = ((1.0 - this->radius)-this->x)/this->vx;
	}else if(this->vx < 0){
		time = -(this->x + (1.0 - this->radius))/this->vx;
	}else{
		time = INT_MAX;
	}
	return time;
}

double timeToHitHorizontalWall(Particle* this){
	double time;
	if(this->vy > 0){
		time = ((1.0 - this->radius) - this->y)/this->vy;
	}else if(this->vy < 0){
		time = -(this->y + (1.0-this->radius))/this->vy;
	}else{
		time = INT_MAX;
	}
	return time;
}

void bounceOff(Particle* this, Particle* that){
	double dx = that->x - this->x; 
	double dy = that->y - this->y;
	double dvx = that->vx - this->vx;
	double dvy = that->vy - this->vy;
	double dvdr = dx*dvx + dy*dvy;
	double dist = this->radius + that->radius;
	double J = 2 * this->mass * that->mass * dvdr / ((this->mass + that->mass) * dist);
	double Jx = J * dx / dist;
	double Jy = J * dy / dist;
	this->vx += Jx / this->mass;
	this->vy += Jy / this->mass;
	that->vx -= Jx / that->mass;
	that->vy -= Jy / that->mass;
	this->collisions++;
	that->collisions++;
}

void bounceOffVerticalWall(Particle* this){
	this->vx *= -1;
	this->collisions++;
}

void bounceOffHorizontalWall(Particle* this){
	this->vy *= -1;
	this->collisions++;
}


