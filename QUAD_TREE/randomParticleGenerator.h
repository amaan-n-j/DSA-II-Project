#ifndef RANDOM_PARTICLE_GEN_
#define RANDOM_PARTICLE_GEN_

#include "particle.h"

#define LEFT_BOUNDARY -100
#define RIGHT_BOUNDARY 100
#define BOTTOM_BOUNDARY -100
#define TOP_BOUNDARY 100
#define MAX_VELOCITY_COMPONENT 2.0
#define MAX_MASS 0.3
#define MIN_MASS 0.1
#define MAX_RADIUS 2.0
#define MIN_RADIUS 2.0
#define PARTICLE_COUNT 100

typedef struct particleSystem{
	Particle* particleArray;
	int particleCount;
	//double t;
}particleSystem;

void createRandomSystem(particleSystem** system, int particleCount);

void createRandomSystemFromFile(particleSystem** system, char* file);

double distance(double x1, double y1, double x2, double y2);

int checkOverlap(Particle* particles, int numParticles, int currentIndex, int arrayPos);

double getRandomDouble(double min, double max);

void divideAndAssignParticles(int particleCount, double left, double right, double top, double bottom, Particle* particles, int arrayPos);

#endif
