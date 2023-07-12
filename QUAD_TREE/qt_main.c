#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "randomParticleGenerator.h"
#include "particle.h"
#include "quadtree.h"
//#include"pq_collisionSystem.h"
//#define PQ_PARTICLE pq_sys.sys
// Window size
int width = 1000, height = 1000;
int iterations = 0;
int max_iterations = 100;
double total_comparision = 0;

//pq_CollisionSystem pq_sys;
particleSystem* qt_sys;
quadtree_node* QT;

void drawCircle(float r, float x, float y);

// Bouncing ball function
void drawScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
	
    // Draw the ball
    Particle particle;
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
    	particle = qt_sys->particleArray[i];
    	glColor3f(particle.color.r, particle.color.g, particle.color.b);
   	drawCircle(particle.radius, particle.x, particle.y);
    }
    // Swap buffers
    glutSwapBuffers();
}

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

// Timer function for ball movement
void update() {
    // Update the ball position
    //printf("current iteration: %d\n",iterations);
    iterations ++;
    double comparisons = 0;


    double dt = (MIN_RADIUS);
    dt /= (MAX_VELOCITY_COMPONENT);

    propagate_sys(qt_sys, dt);
    reverse_at_boundry(qt_sys);
    clearQuadtree(QT);
    //QT = createquadtree_node(0, 0, 2, 2);
    QT = createquadtree_node(0, 0, RIGHT_BOUNDARY - LEFT_BOUNDARY, TOP_BOUNDARY - BOTTOM_BOUNDARY);
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
        insertParticleQuadtree(&QT, &(qt_sys->particleArray[i]));
    }
    //updateQuadtree(&QT, &QT);
    if(!correct_quadtree(QT))
        printf("WRONG quadtree\n");
    comparisons = detectCollisionQuadtree(qt_sys, &QT, comparisons);  
    //printf("%lf comparisons performed\n",comparisons);
    total_comparision += comparisons;

//    if (iterations > max_iterations) {
//        total_comparision /= (iterations - 1);
//        printf("***** SUMMARY *****\n");
//        printf("No. of particles: %d\n",PARTICLE_COUNT);
//        printf("max particles per quadtree node: %d\n",MAX_PARTICLES);
//        printf("size range: %f - %f\n",MIN_RADIUS, MAX_RADIUS);
//        printf("velocity range: 0 - %f\n", MAX_VELOCITY_COMPONENT);
//        printf("total iterations: %d\n", iterations-1);
//        printf("average comparisons per iteration: %lf\n",total_comparision);
//        // Exit the main loop
//        glutLeaveMainLoop();
//    }

    // Redraw the scene
    glutPostRedisplay();


    // Set the timer for the next update
    glutTimerFunc(16, update, 0);
}

void keyboardFunc(unsigned char key, int x, int y) {
    if (key == 'x' || key == 'X') {
        total_comparision /= (iterations - 1);
        printf("***** SUMMARY *****\n");
        printf("No. of particles: %d\n",PARTICLE_COUNT);
        printf("max particles per quadtree node: %d\n",MAX_PARTICLES);
        printf("size range: %f - %f\n",MIN_RADIUS, MAX_RADIUS);
        printf("velocity range: 0 - %f\n", MAX_VELOCITY_COMPONENT);
        printf("total iterations: %d\n", iterations-1);
        printf("average comparisons per iteration: %lf\n",total_comparision);
        // Exit the main loop
        glutLeaveMainLoop();
    }
}

// Main function
int main(int argc, char* argv[]) {	
    QT = createquadtree_node(0, 0, RIGHT_BOUNDARY - LEFT_BOUNDARY, TOP_BOUNDARY - BOTTOM_BOUNDARY);

    if(argc == 1){
	    createRandomSystem(&(qt_sys), PARTICLE_COUNT);
    }
    else if(argc == 2){
        createRandomSystemFromFile(&(qt_sys), argv[1]);
    }
    else{
        printf("usage: ./sim <optional file argument>");
        return 1;
    }

    printf("particle array size id: %d\n",qt_sys->particleCount);
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
    	//particle = qt_sys->particleArray[i];
        insertParticleQuadtree(&QT, &(qt_sys->particleArray[i]));
    }
//    display_QT(QT);


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
 	gluOrtho2D(-100.0f, 100.0f, -100.0f, 100.0f);
 	   
 	// Set the display function and timer
 	glutDisplayFunc(drawScene);
	glutTimerFunc(16, update, 0);
 	glutKeyboardFunc(keyboardFunc);
    // Start the main loop
 	glutMainLoop();
 	return 0;
}
