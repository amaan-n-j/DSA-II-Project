#ifndef PARTICLE_H_
#define PARTICLE_H_
typedef struct Colour{
	float r,g,b;
} Color;

typedef struct Particle{
	double x,y,vx,vy,radius,mass;
	int collisions;
	Color color;
} Particle;

Particle* newParticle(double x, double y, double vx, double vy, double radius, double mass, Color color);

void draw(Particle* p);

void move(Particle* p, double dt);

double timeToHit(Particle* this, Particle* that);
double timeToHitVerticalWall(Particle* this);
double timeToHitHorizontalWall(Particle* this);

void bounceOff(Particle* this, Particle* that);
void bounceOffHorizontalWall(Particle* this);
void bounceOffVerticalWall(Particle* this);

#endif

