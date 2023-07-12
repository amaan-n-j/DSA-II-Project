#ifndef RANDOM_PARTICLE_GEN_
#define RANDOM_PARTICLE_GEN_

#include"../include/KD_Helping_func.h"

typedef struct particleSystem{
	Particle* particleArray;
	int particleCount;
}particleSystem;


void createRandomSystem(particleSystem** system, int particleCount);

double distance(double x1, double y1, double x2, double y2);

int checkOverlap(Particle* particles, int numParticles, int currentIndex, int arrayPos);

double getRandomDouble(double min, double max);

void divideAndAssignParticles(int particleCount, double left, double right, double top, double bottom, Particle* particles, int arrayPos);

#endif
