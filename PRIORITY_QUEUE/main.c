/*#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"pq_collisionSystem.h"
#define PQ_PARTICLE pq_sys.sys
int width = 1000, height = 1000;

pq_CollisionSystem pq_sys;

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

void drawScene() {
    	// Clear the screen
    	glClear(GL_COLOR_BUFFER_BIT);
	
    	// Draw the particle
    	Particle particle;
    	for(int i = 0 ; i < PQ_PARTICLE->particleCount ; i++){
    		particle = PQ_PARTICLE->particleArray[i];
    		glColor3f(particle.color.r, particle.color.g, particle.color.b);
   		drawCircle(particle.radius, particle.x, particle.y);
    	}
    	// Swap buffers
    	glutSwapBuffers();
}

// Timer function for ball movement
void update(int value) {
    	// Update the ball position
    	Particle particle;
    	double ballRadius;// ballX, ballY, ballXSpeed, ballYSpeed
    	for(int i = 0 ; i < PQ_PARTICLE->particleCount ; i++){
    		particle = PQ_PARTICLE->particleArray[i];
    		ballRadius = particle.radius;
    	    	move(&(PQ_PARTICLE->particleArray[i]),0.5);
    	     // Check for collision with window edges
    	    	if (PQ_PARTICLE->particleArray[i].x > 1.0f - ballRadius || PQ_PARTICLE->particleArray[i].x < -1.0f + ballRadius) {
    	    		bounceOffVerticalWall(&(PQ_PARTICLE->particleArray[i]));
    	     	}
    		if (PQ_PARTICLE->particleArray[i].y > 1.0f - ballRadius || PQ_PARTICLE->particleArray[i].y < -1.0f + ballRadius) {
    	    		bounceOffHorizontalWall(&(PQ_PARTICLE->particleArray[i]));
    		} 	
    	    
    	}
    	 // Redraw the scene
    	glutPostRedisplay();

    	// Set the timer for the next update
    	glutTimerFunc(16, update, 0);
}

void keyboardFunc(unsigned char key, int x, int y) {
    	if (key == 'x' || key == 'X') {
        	// Exit the main loop
        	glutLeaveMainLoop();
    	}
}

// Main function
int main(int argc, char** argv) {	
	createRandomSystem(&(pq_sys.sys), 200);
	// Initialize GLUT and create a window
   	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   	glutInitWindowSize(width, height);
    	glutCreateWindow("Bouncing Ball");
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
}*/
