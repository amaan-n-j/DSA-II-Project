#include<stdlib.h>
#include<stdio.h>
#include"../include/randomParticleGenerator.h"
#include<math.h>
#include<time.h>

void createRandomSystem(particleSystem** system, int particleCount){
	*system = (particleSystem*)malloc(sizeof(particleSystem));
	if(!(*system)){
		printf("Malloc in generation of random system failed");
		return;
	}
	(*system)->particleCount = particleCount;
	(*system)->particleArray = (Particle*)malloc(sizeof(Particle)*particleCount);
	divideAndAssignParticles(particleCount, -1, 1, -1, 1, (*system)->particleArray, 0);
}

double distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int checkOverlap(Particle* particles, int numParticles, int currentIndex, int arrayPos) {
	double currentX = particles[currentIndex].x;
	double currentY = particles[currentIndex].y;
	double currentRadius = particles[currentIndex].radius;
	
   	for (int i = arrayPos; i < numParticles+arrayPos; i++) {
       	 	if (i == currentIndex) {
			continue;
        	}

        	double otherX = particles[i].x;
        	double otherY = particles[i].y;
        	double otherRadius = particles[i].radius;

        	double dist = distance(currentX, currentY, otherX, otherY);
        	double minDist = currentRadius + otherRadius;

        	if (dist < minDist) {
		//	printf("Overlap with %d\n", i);
			return 1; // Overlap detected
        	}
	}
	return 0; // No overlap
}

double getRandomDouble(double min, double max) {
	double range = max - min;
    	double randomValue = ((double)rand() / RAND_MAX) * range + min;
    	return randomValue;
}

void divideAndAssignParticles(int particleCount, double left, double right, double bottom, double top, Particle* particles, int arrayPos){
	if(particleCount <= 40){
	//	printf("%d %lf %lf %lf %lf %d\n",particleCount, left, right, bottom, top, arrayPos);
		for (int i = arrayPos; i < arrayPos+particleCount; i++){
	        	particles[i].radius = getRandomDouble(0.02,0.05);
			particles[i].x = getRandomDouble(left + particles[i].radius, right - particles[i].radius);
	       		particles[i].y = getRandomDouble(bottom + particles[i].radius, top - particles[i].radius);
			//printf("%d x = %lf y = %lf\n",i,particles[i].x,particles[i].y);
	       	 	particles[i].vx = getRandomDouble(-0.02,0.02);
	        	particles[i].vy = getRandomDouble(-0.02,0.02);
	        	particles[i].mass = getRandomDouble(0.1,10);
	        	particles[i].collisions = 0;
		        particles[i].color.r = getRandomDouble(0,1);
		        particles[i].color.g = getRandomDouble(0,1); 
			particles[i].color.b = getRandomDouble(0,1);
	
	        // Check for overlaps and reassign random values if necessary
	        	while (checkOverlap(particles, particleCount, i, arrayPos)) {
				//srand(time(0));
			//	printf("**%d**\n",i);
			//	printf("Before:");
			//	printf("%f %f %f",particles[i].x, particles[i].y, particles[i].radius);
	            		particles[i].radius = getRandomDouble(0.02,0.05);
				particles[i].x = getRandomDouble(left + particles[i].radius, right - particles[i].radius);
	       			particles[i].y = getRandomDouble(bottom + particles[i].radius, top - particles[i].radius);
			//	printf("\nAfter:");
			//	printf("%f %f %f\n",particles[i].x, particles[i].y, particles[i].radius);
	        	}
		}
	}else{
		//for top left region
		divideAndAssignParticles(particleCount/4, left, (left+right)/2, (top+bottom)/2, top, particles, arrayPos);
		//for top right region
		divideAndAssignParticles(particleCount/4, (left+right)/2, right, (top+bottom)/2, top, particles, arrayPos+(particleCount/4));
		// for bottom left region
		divideAndAssignParticles(particleCount/4, left, (left+right)/2, bottom, (top+bottom)/2, particles, arrayPos+2*(particleCount/4));
		//for bottom right region
		divideAndAssignParticles(particleCount/4, (left+right)/2, right, bottom, (top+bottom)/2, particles, arrayPos+3*(particleCount/4));
	}
}

//void displaySystemConfiguration(CollisionSystem system){
//	Particle* particleArray = system->particleArray;
//	int count = system->particleCount;
//	for(int i = 0;i<particleCount;i++){
//		printf("Particle %d - x:")
//	}
//}


