#include "application.h"

#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>

/* WORKAROUND FOR (K)UBUNTU */
/* WRONG LINK TO PTHREAD CAUSES CRASH ON STARTUP IN C++11 MODE */
#include <pthread.h>
void* simpleFunc(void*) { return NULL; } void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }
/* WORKAROUND FOR (K)UBUNTU */

void opengl_on_timer(int)
{
	glutPostRedisplay(); // Redraw screen with new object data.
	glutTimerFunc(10, &opengl_on_timer, 1);
}

void opengl_on_draw()
{
	Application::getInstance()->draw();

	//Draw order
	glutSwapBuffers();
}

//Main program
int main(int argc, char **argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);

	//Simple buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
	// glutInitWindowPosition(150, 125);
	glutInitWindowSize(Application::SCREEN_W, Application::SCREEN_H);
	glutCreateWindow("OpenGL");

	if (!Application::getInstance()->init())
		return 1;

	glutTimerFunc(10, &opengl_on_timer, 1);
	glutDisplayFunc(&opengl_on_draw);
	glutMainLoop();

	return 0;
}
