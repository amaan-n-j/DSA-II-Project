#ifndef KD_TREE_H_
#define KD_TREE_H_

typedef struct Colour{
	float r,g,b;
} Color;

typedef struct Particle{
	double x,y,vx,vy,radius,mass;
	int collisions;
	Color color;
} Particle;


typedef struct kdnode{
	Particle ball;
  	struct kdnode *left , *right;
}kdnode;

typedef kdnode* kdtree;

#endif

kdtree createNode(Particle point);

int comparePoints(Particle p1, Particle p2, int level);

void swap(Particle *a, Particle *b);

int partition_xy(Particle array[], int low, int high , int*flag);

void quickSort_xy(Particle array[], int low, int high, int*flag);

double calculateDistance(Particle p1,Particle p2);
 
void displayKDTree(kdtree root);

void Pre_order(kdtree root);

void printArray(Particle array[], int size);

