#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../include/KD_Helping_func.h"

kdtree createNode(Particle point){
	kdtree kd;
	kd  = (kdtree)malloc(sizeof(kdnode));
	kd->ball.x = point.x;
	kd->ball.y = point.y;
	kd->ball.mass = point.mass;
	kd->ball.radius = point.radius;
	kd->ball.vx = point.vx;
	kd->ball.vy = point.vy;
	kd->ball.color.r = point.color.r;
	kd->ball.color.g = point.color.g;
	kd->ball.color.b = point.color.b;
	kd->left = NULL;	
	kd->right = NULL;
	return kd;
}

int comparePoints(Particle p1, Particle p2, int level) {
    if (level % 2 == 0) {
        if (p1.x < p2.x)
            return -1;
        else if (p1.x > p2.x)
            return 1;
        else
            return 0;
    } else {
        if (p1.y < p2.y)
            return -1;
        else if (p1.y > p2.y)
            return 1;
        else
            return 0;
    }
}

void swap(Particle *a, Particle *b) {
  Particle t = *a;
  *a = *b;
  *b = t;
}

int partition_xy(Particle array[], int low, int high , int*flag) {
  	int pivot;
 	if(*flag) pivot = array[high].y;
 	else pivot = array[high].x;

	int i = (low - 1);

  	for (int j = low; j < high; j++) {
    		if(*flag){
			if (array[j].y <= pivot) {
     	 			i++;
     				swap(&array[i], &array[j]);
    			}
    		}	
		else{
    			if (array[j].x <= pivot) {
     	 			i++;
     				swap(&array[i], &array[j]);
    			}
		}
  	}	

  	swap(&array[i + 1], &array[high]);
  
  	return (i + 1);
}

void quickSort_xy(Particle array[], int low, int high, int*flag) {
  if (low < high) {

    int pi = partition_xy(array, low, high,flag);
    
    quickSort_xy(array, low, pi - 1,flag);
   
    quickSort_xy(array, pi + 1, high,flag);
  }
}

double calculateDistance(Particle p1,Particle p2){
	return ( pow( (p1.x - p2.x), 2)  + pow( (p1.y - p2.y),2 ) );
}

// function to print array elements
void printArray(Particle array[], int size) {
	for (int i = 0; i < size; ++i) {
		printf(" x = %lf \t y = %lf \n", array[i].x,array[i].y);
	}
}

//Inorder Traversal 
//@param - tree
void displayKDTree(kdtree root) {
	if (root == NULL)
		return;

	displayKDTree(root->left);
	printf("%.2f\t%.2f\t%.2f\n", root->ball.x, root->ball.y,root->ball.color.r);
	displayKDTree(root->right);
	return;
}

//Preorder Trraversal
//@param - tree
void Pre_order(kdtree root) {
	if (root == NULL)
		return;

	printf("(%.2f, %.2f)\n", root->ball.x, root->ball.y);
	Pre_order(root->left);
	Pre_order(root->right);
	return;
}


