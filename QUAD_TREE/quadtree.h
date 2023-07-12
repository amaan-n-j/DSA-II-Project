#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "particle.h"
#include "randomParticleGenerator.h"
#define MAX_PARTICLES (PARTICLE_COUNT / 2)

typedef struct quadtree_node{
    double x;
    double y;
    double width;
    double height;
    int particle_count;
    //int level;
    Particle* particles[MAX_PARTICLES];
    struct quadtree_node* children[4];
    // index: 0 = south-west, 1 = south-east, 2 = north-west, 3 = north-east
}quadtree_node;

// Function to create a new quadtree node
quadtree_node* createquadtree_node(double x, double y, double width, double height) {
    quadtree_node* node = (quadtree_node*)malloc(sizeof(quadtree_node));
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    //node->level = 0;
    node->particle_count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        node->particles[i] = NULL;
    }
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Function to check if a node is a leaf node (has no children)
bool isLeafNode(quadtree_node** node) {
    return ((*node)->children[0] == NULL);
}

// clamp function
double clamp(double val, double minval, double maxval){
    return fmax(fmin(val, maxval), minval);
}

// Function to check if a point is cutting a region
// param in: center of point (x,y), center of region (x,y), radius of particle, width_region, height_region
bool isCuttingRegion(double Cx, double Cy, double Rx, double Ry, double radius, double width, double height){
    double dist = 0;
    double dx, dy;
    double x1 = Rx - width / 2;
    double y1 = Ry - height / 2;
    double x2 = Rx + width / 2;
    double y2 = Ry + height / 2;

    dx = clamp(Cx, x1, x2) - Cx;
    dy = clamp(Cy, y1, y2) - Cy;
    dist = sqrt(dx * dx + dy * dy);
    //printf("distance is %lf\n",distance);
    return (dist  <= radius);

}

void subdivideNode(quadtree_node* node) {
    double subWidth = node->width / 2.0;
    double subHeight = node->height / 2.0;
    //printf("new width is %lf, new height is %lf\n",subWidth, subHeight);
//    if(subWidth < 0.001 || subHeight < 0.001){
//        return;
//    }

    node->children[0] = createquadtree_node(node->x - subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[1] = createquadtree_node(node->x + subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[2] = createquadtree_node(node->x - subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);
    node->children[3] = createquadtree_node(node->x + subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);

}

bool particleAlreadyExists(quadtree_node* node, Particle* particle){
    for(int i = 0; i < node->particle_count; i++){
        if(particle == node->particles[i]){
            return true;
        }
    }
    return false;
}

void insertParticleQuadtree(quadtree_node** node, Particle* particle) {

    if ((*node)->children[0] != NULL) {
        // Find the child node that the particle belongs to and insert it there
        double subWidth = (*node)->width / 2.0;
        double subHeight = (*node)->height / 2.0;

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[0], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[1], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[2], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[3], particle);

        return;
    }

    // Add the particle to the node's particle array
    if ((*node)->particle_count < MAX_PARTICLES /*&& (! particleAlreadyExists(*node, particle))*/) {
        (*node)->particles[(*node)->particle_count] = particle;
        (*node)->particle_count++;
        //printf("inserted point %lf, %lf\n",((*node)->particles[(*node)->particle_count++])->x,((*node)->particles[(*node)->particle_count++])->y);
     //   int index = (*node)->particle_count - 1;
     //   printf("inserted point %lf, %lf, radius: %lf\n",particle->x, particle->y, particle->radius);
     //   printf("address of particle is %d\n",particle);
     //   printf("region is: %lf, %lf\n",(*node)->x,(*node)->y);
     //   printf("width: %lf, height: %lf\n\n",(*node)->width,(*node)->height);

    } 
    else {
        // Subdivide the node if it has reached the maximum number of particles
        subdivideNode(*node);

        // Reinsert the existing particles into the children nodes
        for (int i = 0; i < (*node)->particle_count; i++) {
            Particle* existingParticle = (*node)->particles[i];
            insertParticleQuadtree(node, existingParticle);
        }
        //
        // Clear the particle array in the current node
        for (int i = 0; i < MAX_PARTICLES; i++) {
            (*node)->particles[i] = NULL;
        }

        (*node)->particle_count = 0;

        // Insert the new particle into the appropriate child node
        // Find the child node that the particle belongs to and insert it there
        double subWidth = (*node)->width / 2.0;
        double subHeight = (*node)->height / 2.0;

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[0], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[1], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[2], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[3], particle);
    }
}

void propagate_sys(particleSystem* qt_sys, double dt){
    for(int i = 0; i < qt_sys->particleCount; i++){
        Particle* particle = &(qt_sys->particleArray)[i];
        particle->x += particle->vx * dt;
        particle->y += particle->vy * dt;
    }
}

void reverse_at_boundry(particleSystem* qt_sys){
    for (int i = 0; i < qt_sys->particleCount; i++) {
        Particle* particle = &(qt_sys->particleArray[i]);

        double left_gap = particle->x - particle->radius - LEFT_BOUNDARY;
        double right_gap = RIGHT_BOUNDARY - particle->x - particle->radius;
        double bottom_gap = particle->y - particle->radius - BOTTOM_BOUNDARY;
        double top_gap = TOP_BOUNDARY - particle->y - particle->radius;

        if(left_gap < 0 && particle->vx < 0){
            //propagate_sys(qt_sys, left_gap / (particle->vx));
            particle->vx *= -1;
            //propagate_sys(qt_sys, -left_gap / (particle->vx));
        }

        else if(right_gap < 0 && particle->vx > 0){
            //propagate_sys(qt_sys, right_gap / (particle->vx));
            particle->vx *= -1;
            //propagate_sys(qt_sys, -right_gap / (particle->vx));
        }

        if(bottom_gap < 0 && particle->vy < 0){
            //propagate_sys(qt_sys, bottom_gap / (particle->vy));
            particle->vy *= -1;
            //propagate_sys(qt_sys, -bottom_gap / (particle->vy));
        }

        else if(top_gap < 0 && particle->vy > 0){
            //propagate_sys(qt_sys, top_gap / (particle->vy));
            particle->vy *= -1;
            //propagate_sys(qt_sys, -top_gap / (particle->vy));
        }

    }

}



void destroyQuadtree(quadtree_node* node) {
    if (node == NULL) {
        return;
    }

    // Recursively destroy the children nodes
    for (int i = 0; i < 4; i++) {
        destroyQuadtree(node->children[i]);
    }

    // Free the memory allocated for the particle array
    for (int i = 0; i < node->particle_count; i++) {
        node->particles[i] = NULL;
    }

    // Free the memory allocated for the current node
    free(node);
}

void clearQuadtree(quadtree_node* node){
    for(int i = 0; i < 4; i++){
        destroyQuadtree(node->children[i]);
    }

    for (int i = 0; i < node->particle_count; i++) {
        node->particles[i] = NULL;
    }
}

int getParticleCountRegion(quadtree_node* node){
    if(node == NULL){
        return 0;
    }

    if(node->children[0] == NULL){
        return node->particle_count;
    }

    int count = 0;
    for(int i = 0; i < 4; i++){
        count += getParticleCountRegion(node->children[i]);
    }
    return count;
}

//void updateQuadtree(quadtree_node** node, quadtree_node** root) {
//    if (*node == NULL) {
//        return;
//    }
//
//    if ((*node)->children[0] == NULL) {
//        for (int i = 0; i < (*node)->particle_count; i++) {
//            Particle* particle = (*node)->particles[i];
//            if (!particle) {
//                break;
//            }
//
//            if (! isCuttingRegion(particle->x, particle->y, (*node)->x, (*node)->y, particle->radius, (*node)->width, (*node)->height)) {
//                (*node)->particles[i] = (*node)->particles[(*node)->particle_count - 1];
//                (*node)->particles[(*node)->particle_count - 1] = NULL;
//                (*node)->particle_count --;
//                insertParticleQuadtree(root, particle);
//            }
//        }
//        return;
//    }
//
//    if(getParticleCountRegion(*node) == 0){
//        destroyQuadtree(*node);
//    }
//    else{
//        updateQuadtree(&(*node)->children[0], root);
//        updateQuadtree(&(*node)->children[1], root);
//        updateQuadtree(&(*node)->children[2], root);
//        updateQuadtree(&(*node)->children[3], root);
//    }
//}

void updateQuadtree(quadtree_node** node, quadtree_node** root) {
    
}

bool isOverlappingParticles(Particle* p1, Particle* p2){
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    double distance = sqrt(dx * dx + dy * dy ); 
    return (distance <= p1->radius + p2->radius);
}

double distanceParticles(Particle* p1, Particle* p2){
    if(p1 == NULL || p2 == NULL){
        return INT_MAX;
    }
    double dx = p1->x - p2->x;
    double dy = p1->y - p2->y;
    double distance = sqrt(dx * dx + dy * dy ); 
    return distance;
}

void bounceOff(Particle* p1, Particle* p2){
	double dx = p2->x - p1->x; 
	double dy = p2->y - p1->y;
	double dvx = p2->vx - p1->vx;
	double dvy = p2->vy - p1->vy;
	double dvdr = dx*dvx + dy*dvy;
	double dist = p1->radius + p2->radius;
	double J = 2 * p1->mass * p2->mass * dvdr / ((p1->mass + p2->mass) * dist);
	double Jx = J * dx / dist;
	double Jy = J * dy / dist;
	p1->vx += Jx / p1->mass;
	p1->vy += Jy / p1->mass;
	p2->vx -= Jx / p2->mass;
	p2->vy -= Jy / p2->mass;
	p1->collisions++;
	p2->collisions++;
}


bool correct_quadtree(quadtree_node* node){
    if(!node)
        return true;
    if(node->children[0] == NULL){
        bool result = true;
        if(node->particle_count == 0){
            return true;
        }
        for(int i = 0; i < node->particle_count; i++){
            Particle* particle = node->particles[i];
            if(particle == NULL){
                return true;
            }
            result = result && isCuttingRegion(particle->x, particle->y, node->x, node->y, particle->radius, node->width, node->height);
        }
        return result;
    }

    bool result = true;
    for(int i = 0; i < 4; i++){
        result = result && correct_quadtree(node->children[0]);
    }
    return result;
}

void handle_collision(Particle* p1, Particle* p2, particleSystem* qt_sys){
    double dx = p1->x - p2->x; 
    double dy = p1->y - p2->y; 
    double distance = distanceParticles(p1, p2);
    double collision_normal_x = dx / distance;
    double collision_normal_y = dy / distance;
    double vx_rel = p2->vx - p1->vx;
    double vy_rel = p2->vy - p1->vy;
    double impact_vel = vx_rel * collision_normal_x + vy_rel * collision_normal_y;
    if(impact_vel > 0){
        double M = p2->mass + p1->mass;
        //double impulse = (2 * impact_vel) / (mass_ratio + 1);
        p1->vx += (2 * p2->mass / M) * impact_vel * collision_normal_x ;
        p1->vy += (2 * p2->mass / M) * impact_vel * collision_normal_y ;
        p2->vx -= (2 * p1->mass / M) * impact_vel * collision_normal_x ;
        p2->vy -= (2 * p1->mass / M) * impact_vel * collision_normal_y ;
    }
   
}

//void detectCollisionQuadtree(particleSystem* qt_sys, quadtree_node** node){
//    if((*node)->children[0] == NULL){
//        for(int i = 0; i < (*node)->particle_count - 1; i++){
//            Particle* p1 = (*node)->particles[i];
//            for(int j = i+1; j < (*node)->particle_count; j++){
//                Particle* p2 = (*node)->particles[j];
//                double distance = distanceParticles(p1, p2);
//                if(distance <= p1->radius + p2->radius){
//                    //printf("overlap detected\n");
//                    handle_collision(p1, p2, qt_sys);
//                    
//                }
////                if (distance == p1->radius + p2->radius){
////                    bounceOff(p1, p2);
////                }
//                    
//            }
//        }
//
//        return;
//    }
//    for(int i = 0; i < 4; i++){
//        detectCollisionQuadtree(qt_sys, &(*node)->children[i]);
//    }
//}

double detectCollisionQuadtree(particleSystem* qt_sys, quadtree_node** node, int comparisons){
    if((*node)->children[0] == NULL){
        for(int i = 0; i < (*node)->particle_count - 1; i++){
            Particle* p1 = (*node)->particles[i];
            for(int j = i+1; j < (*node)->particle_count; j++){
                Particle* p2 = (*node)->particles[j];
                double distance = distanceParticles(p1, p2);
                if(distance <= p1->radius + p2->radius){
                    //printf("overlap detected\n");
                    handle_collision(p1, p2, qt_sys);
                    
                }
//                if (distance == p1->radius + p2->radius){
//                    bounceOff(p1, p2);
//                }
                    
            }
        }

        double n = (*node)->particle_count;
        return (n * (n - 1) / 2);
    }
    for(int i = 0; i < 4; i++){
        comparisons += detectCollisionQuadtree(qt_sys, &(*node)->children[i], comparisons);
    }
    return comparisons;
}

void display_QT(quadtree_node* node){

    if(node->children[0] == NULL){
        for(int i = 0; i < (node->particle_count); i++){
            printf("(%lf , %lf)",(node->particles[i])->x,(node->particles[i])->y);
            //printf("this particle number %d",i);
        }
        printf("\n");
        return;
    }
    //printf("ithe\n");
    display_QT(node->children[1]);
    //printf("ithe 2\n");
    display_QT(node->children[0]);
    //printf("ithe 3\n");
    display_QT(node->children[2]);
    //printf("ithe 4\n");
    display_QT(node->children[3]);
    //printf("ithe 5\n");
    return;

}

#endif
