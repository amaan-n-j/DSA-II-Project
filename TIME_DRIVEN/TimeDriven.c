#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pq_collisionSystem.h"
#include "particle.h"
#include "particle.c"
#include "randomParticleGenerator.c"
#include "randomParticleGenerator.h"

#define PQ_PARTICLE pq_sys.sys
#define WIDTH 1000
#define HEIGHT 1000
#define PARTICLE_NUM 800
int width = 1000, height = 1000;

void glutPostRedisplay(void);
void glutTimerFunc(unsigned int millis, void (*timerFunc)(int value), int value);

pq_CollisionSystem pq_sys;

//Function to draw circle
void drawCircle(float r, float x, float y) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (float i = 0; i < 360; i++) {
        glVertex2f(x + r * cos(i * 180 / M_PI), y + r * sin(i * 180 / M_PI));
    }
    glEnd();
}

void drawScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the particles
    Particle particle;
    for (int i = 0; i < PQ_PARTICLE->particleCount; i++) {
        particle = PQ_PARTICLE->particleArray[i];
        glColor3f(particle.color.r, particle.color.g, particle.color.b);
        drawCircle(particle.radius, particle.x, particle.y);
    }

    // Swap buffers
    glutSwapBuffers();
}

double calculateDistance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

// Timer function for particle movement
void update(int value) {
    Particle particle;
    double ballRadius;
	int z=0;
	int u=0;

    for (int i = 0; i < PQ_PARTICLE->particleCount; i++) {
        particle = PQ_PARTICLE->particleArray[i];
        ballRadius = particle.radius;

        double* x = &(PQ_PARTICLE->particleArray[i].x);
        double* y = &(PQ_PARTICLE->particleArray[i].y);
        double* vx = &(PQ_PARTICLE->particleArray[i].vx);
        double* vy = &(PQ_PARTICLE->particleArray[i].vy);
        double* radius = &(PQ_PARTICLE->particleArray[i].radius);

        *x += *vx;
        *y += *vy;

        // Check for collision with window edges

        // Check for vertical edges
        if ((*x - *radius <= -1 && *vx < 0) || (*x + *radius >= 1 && *vx > 0)) {
            *vx *= -1;
        }

        // Check for horizontal edges
        if ((*y - *radius <= -1 && *vy < 0) || (*y + *radius >= 1 && *vy > 0)) {
            *vy *= -1;
        }
		z++;
        // Check for Particle collision
        for (int j = 0; j < PQ_PARTICLE->particleCount; j++) {
            Particle* otherParticle = &(PQ_PARTICLE->particleArray[j]);

            double distance = calculateDistance(*x, *y, otherParticle->x, otherParticle->y);
            double sumOfRadii = *radius + otherParticle->radius;

            if (distance == sumOfRadii) {
                bounceOff(&PQ_PARTICLE->particleArray[i], otherParticle);
            }

            if (distance < sumOfRadii) {
                float dx = *x - otherParticle->x;
                float dy = *y - otherParticle->y;

                float collisionNormalX = dx / distance;
                float collisionNormalY = dy / distance;

                float relativeVelocityX = otherParticle->vx - *vx;
                float relativeVelocityY = otherParticle->vy - *vy;

                float impactVelocity = relativeVelocityX * collisionNormalX + relativeVelocityY * collisionNormalY;

                if (impactVelocity > 0) {
                    float massRatio1 = otherParticle->mass / particle.mass;
                    float massRatio2 = 1.0f / massRatio1;

                    float impulse1 = (2.0f * impactVelocity) / (massRatio1 + 1.0f);
                    float impulse2 = (2.0f * impactVelocity) / (massRatio2 + 1.0f);

                    *vx += impulse1 * collisionNormalX;
                    *vy += impulse1 * collisionNormalY;
                    otherParticle->vx -= impulse2 * collisionNormalX;
                    otherParticle->vy -= impulse2 * collisionNormalY;
                }
				u++;
            }
        }
    }
    int comp = z*u;
    printf("%d\n",comp);

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
    createRandomSystem(&(pq_sys.sys), PARTICLE_NUM);

    // Initialize GLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Bouncing Ball");

    // Set the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

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

