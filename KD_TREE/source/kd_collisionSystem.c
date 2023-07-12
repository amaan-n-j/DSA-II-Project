#include<stdlib.h>
#include<stdio.h>
#include"../include/kd_collisionSystem.h"
#include<math.h>
#include<time.h>

// Create the tree
// @param - Particle array
// @param - start point of the array
// @param - end point of the array
// @param - level /depth - intially =  0
kdtree createKDTree(Particle points[],int start , int end , int level) {
	if (start > end) return NULL;
	// Calculate the current dimension (level)
	int currentLevel = level % 2;

	// Sort the points array based on the current level
	if (currentLevel == 0) {
		quickSort_xy(points,start,end,&currentLevel);
	}
	else {	
		quickSort_xy(points,start,end,&currentLevel);
	} 		

	// Find the median index
	int medianIndex;
	if((end - start) % 2) medianIndex = ((end-start)/2) + 1 + start;
	else medianIndex = start +  (end-start)/2 ;

	kdtree root;
	root = NULL;	
	// Create a new node with the median point
	root = createNode(points[medianIndex]);

	// Recursively construct the left and right subtrees
	root->left = createKDTree(points, start, medianIndex - 1, level + 1);
	root->right = createKDTree(points, medianIndex + 1, end, level + 1);

	return root;	   
}


//Collision check between the paritcles 
//@param - particle instance - p1
//@param - particle instance - p2
int collisionCheck_P2P(Particle p1,Particle p2){
	double dist = sqrt(calculateDistance(p1,p2));
	if(dist <= (p1.radius + p2.radius)) return 1;
	return 0;
}

//Updating the tree using the array
//@param - kdtree pointer i.e root node
//@param - Particle array
//@param - start point of the array
//@param - end point of the array
//@param - level /depth - intially =  0
kdtree update_tree(kdtree root,Particle points[],int start , int end , int level) {
	if (start > end) return NULL;
	int currentLevel = level % 2;

	if (currentLevel == 0) {
		quickSort_xy(points,start,end,&currentLevel);
	}
	else {	
		quickSort_xy(points,start,end,&currentLevel);
	} 		

	int medianIndex;
	if((end - start) % 2) medianIndex = ((end-start)/2) + 1 + start;
	else medianIndex = start +  (end-start)/2 ;

	root->ball = points[medianIndex]; 

	root->left = update_tree(root->left,points, start, medianIndex - 1, level + 1);
	root->right = update_tree(root->right,points, medianIndex + 1, end, level + 1);

	return root;	   
}

//Helping function for finding the nearest neighbor
//@param - kdnode pointer node-1
//@param - kdnode pointer node-2
//@param - Particle pointer
kdtree closest(kdtree node1, kdtree node2, Particle* particle){
	if( node1 == NULL) return node2;
	
	if( node2 == NULL) return node1;

	double d1 = calculateDistance(node1->ball,*particle);
	
	double d2 = calculateDistance(node2->ball,*particle);

	if (d1 == 0) return node2;

	if (d2 == 0) return node1;


	if( d1 < d2) return node1;
	else return node2;
}

// Function to find the nearest neighbor in the KD-tree
// @param - knode pointer
// @param - paritcle pointer
// @param - depth/level
kdtree findNN(kdtree root,Particle* particle, int depth) {
	if(root == NULL) return NULL;
	kdtree temp = NULL;
	kdtree best = NULL;
	kdtree nextsubtree = NULL;
	kdtree othersubtree = NULL;
	int axis = depth%2;
	if(axis == 0){
		if(particle->x < root->ball.x){
			nextsubtree = root->left;
			othersubtree = root->right;
		}
		else{
			nextsubtree = root->right;
			othersubtree = root->left;
		}
	}
	else{
		if(particle->y < root->ball.y){
			nextsubtree = root->left;
			othersubtree = root->right;
		}
		else{
			nextsubtree = root->right;
			othersubtree = root->left;
		}
	}

	temp = findNN(nextsubtree,particle,depth+1);
	best = closest(temp,root,particle);

	double radius_square = calculateDistance(*particle,best->ball);	

	double dist;
	if (axis == 0) {
		dist = fabs(root->ball.x - particle->x);
	} else {
		dist= fabs(root->ball.y - particle->y);
	}

	if( radius_square >= dist * dist){
		temp = findNN(othersubtree,particle,depth+1);
		best = closest(temp,best,particle);
	}

	return best;

}
	

//Destroys the kdtree
//@param  - knode pointer
void destroyKDTree(kdtree root) {
	if (root == NULL)
		return;

	destroyKDTree(root->left);
	destroyKDTree(root->right);

	free(root);
}

//Function to handle the collision between the particle 
//@param  - Particle pointer - p1
//@param  - Particle pointer - p2
void handle_collision(Particle* p1, Particle* p2){
    double dx = p1->x - p2->x; 
    double dy = p1->y - p2->y; 
    double distance = sqrt(calculateDistance(*p1, *p2));
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

//Helping Function for rebalancing tree
//@param - pointers to tree
//@param - Particle pointer to store the new particles temporary
//@param - index to add element in the array
void collectPointsFromKDTree(kdtree  node, Particle* collectedPoints, int* index) {
    if (node == NULL) {
        return;
    }

    // Traverse left subtree
    collectPointsFromKDTree(node->left, collectedPoints, index);

    // Add current point to the collected points array
    collectedPoints[*index] = node->ball;
    (*index)++;

    // Traverse right subtree
    collectPointsFromKDTree(node->right, collectedPoints, index);
}

//Rebalancing the KDTREE
//@param - kdtree pointer
//@param - number of particles
void rebalanceKDTree(kdtree root ,int numParticles) {
    // Step 1: Traverse the KD tree and collect points in an array
    Particle* collectedPoints = (Particle*)malloc(sizeof(Particle) * numParticles);
    int index = 0;
    collectPointsFromKDTree(root, collectedPoints, &index);

    root = update_tree(root,collectedPoints, 0,numParticles-1,0);

    // Free the collected points array
    free(collectedPoints);
}

void move(Particle* p, double dt){
	p->x += p->vx * dt;
	p->y += p->vy * dt;
}

void move_particles(kdtree root,double k){
	if(root == NULL) return;
	//UPDATING AND COLLISION HANDELLING
	move(&(root->ball),k);
	move_particles(root->left,k);
	move_particles(root->right,k);
}


void update_particles_boundary(kdtree root){
	if(!root) return ;
	
	double left_gap = root->ball.x - root->ball.radius + 1;
        double right_gap = 1 - root->ball.x - root->ball.radius;
        double bottom_gap = root->ball.y - root->ball.radius + 1;
        double top_gap = 1 - root->ball.y - root->ball.radius;

	if(left_gap < 0 && root->ball.vx < 0){
            //propagate_sys(qt_sys, left_gap / (root->ball.vx));
            root->ball.vx *= -1;
            //propagate_sys(qt_sys, -left_gap / (root->ball.vx));
        }

        else if(right_gap < 0 && root->ball.vx > 0){
            //propagate_sys(qt_sys, right_gap / (root->ball.vx));
            root->ball.vx *= -1;
            //propagate_sys(qt_sys, -right_gap / (root->ball.vx));
        }

        if(bottom_gap < 0 && root->ball.vy < 0){
            //propagate_sys(qt_sys, bottom_gap / (root->ball.vy));
            root->ball.vy *= -1;
            //propagate_sys(qt_sys, -bottom_gap / (root->ball.vy));
        }

        else if(top_gap < 0 && root->ball.vy > 0){
            //propagate_sys(qt_sys, top_gap / (root->ball.vy));
            root->ball.vy *= -1;
            //propagate_sys(qt_sys, -top_gap / (root->ball.vy));
        }
	
	update_particles_boundary(root->left);
	update_particles_boundary(root->right);

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

        double left_gap = particle->x - particle->radius + 1;
        double right_gap = 1 - particle->x - particle->radius;
        double bottom_gap = particle->y - particle->radius + 1;
        double top_gap = 1 - particle->y - particle->radius;

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

/*
int main(){
	Particle p_arr[6];
	p_arr[0].x = 5;
	p_arr[0].y = 4;
	p_arr[1].x = 2;
	p_arr[1].y = 6;
	p_arr[2].x = 13;
	p_arr[2].y = 3;
	p_arr[3].x = 3;
	p_arr[3].y = 1;
	p_arr[4].x = 10;
	p_arr[4].y = 2;
	p_arr[5].x = 8;
	p_arr[5].y = 7;


	kdtree tree;
	kdtree nearest;
	tree = createKDTree(p_arr,0,5,0);	
	displayKDTree(tree);
	printf("\n----------------\n");
	printf("x:%.2f , y:%.2f\n", p_arr[0].x,p_arr[0].y);
	nearest = findNN(tree,&p_arr[0],0);
	printf("x:%.2f , y:%.2f\n",nearest->ball.x,nearest->ball.y);
	Pre_order(tree);

	return 0;
}
*/
