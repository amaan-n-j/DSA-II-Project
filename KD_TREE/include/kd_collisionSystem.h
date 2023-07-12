#ifndef PQ_COL_SYS
#define PQ_COL_SYS

#include"../include/KD_Helping_func.h"
#include"../include/randomParticleGenerator.h"

typedef kdnode* kdtree;

typedef struct kd_CollisionSystem{
	kdtree tree;
    	particleSystem* sys;
}KD_CollisionSystem;



kdtree createKDTree(Particle points[],int start , int end , int level);

int collisionCheck_P2P(Particle p1,Particle p2);

kdtree update_tree(kdtree root,Particle points[],int start , int end , int level);

void destroyKDTree(kdtree root);

kdtree findNN(kdtree root,Particle* particle,  int depth) ;

void handle_collision(Particle* p1, Particle* p2);

void rebalanceKDTree(kdtree root ,int numParticles);

void move_particles(kdtree root,double k);

void update_particles_boundary(kdtree root);

void propagate_sys(particleSystem* qt_sys, double dt);

void reverse_at_boundry(particleSystem* qt_sys);

#endif
