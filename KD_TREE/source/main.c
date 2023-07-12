#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"../include/kd_collisionSystem.h"
#define KD_PARTICLE kd_sys.sys
int width = 1000, height = 1000;

KD_CollisionSystem kd_sys;

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

void  particle_collision_render(kdtree root){
	if(root == NULL) return;

	//P2P Collision
	kdtree nearest;
	nearest = findNN(kd_sys.tree,&(root->ball),0);

	if(collisionCheck_P2P(nearest->ball,root->ball)){
		handle_collision(&(nearest->ball), &(root->ball));
	}

	particle_collision_render(root->left);
	particle_collision_render(root->right);
}

void drawKDTree(kdtree root) {
	if (root == NULL)
		return;
	drawKDTree(root->left);
	glColor3f(root->ball.color.r, root->ball.color.g, root->ball.color.b);
	drawCircle(root->ball.radius, root->ball.x, root->ball.y);
	drawKDTree(root->right);
	return;
}

void drawScene() {
    	// Clear the screen
    	glClear(GL_COLOR_BUFFER_BIT);
	
    	// Draw the particle
    	drawKDTree(kd_sys.tree);

    	// Swap buffers
    	glutSwapBuffers();
}

// Timer function for ball movement
void update(int value) {
    	// Update the ball position
	move_particles(kd_sys.tree,0.5);
	update_particles_boundary(kd_sys.tree);
	rebalanceKDTree(kd_sys.tree,KD_PARTICLE->particleCount);
   	particle_collision_render(kd_sys.tree);

    	 // Redraw the scene
    	glutPostRedisplay();

    	// Set the timer for the next update
    	glutTimerFunc(8, update, 0);
}

void keyboardFunc(unsigned char key, int x, int y) {
    	if (key == 'x' || key == 'X') {
        	// Exit the main loop
        	glutLeaveMainLoop();
    	}
}

// Main function
int main(int argc, char** argv) {	
	
	createRandomSystem(&(kd_sys.sys), 50);
	kd_sys.tree = createKDTree(KD_PARTICLE->particleArray,0,KD_PARTICLE->particleCount-1,0);
	
//	displayKDTree(kd_sys.tree);

	// Initialize GLUT and create a window
   	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   	glutInitWindowSize(width, height);
    	glutCreateWindow("SIMULATION : KD-TREE");
   	// Set the background color
 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 	   
 	// Set the projection
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
 	   
 	// Set the display function and timer
 	glutDisplayFunc(drawScene);
	glutTimerFunc(16, update, 0);
 	glutKeyboardFunc(keyboardFunc);
 	// Start the main loop
 	glutMainLoop();
 	return 0;
}
