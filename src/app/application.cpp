#include "application.h"

#include <opengl-stb/stbgl_shader.h>
#include <opengl-stb/stbgl_surface.h>

#include <string>
#include <png.h>
#include <GLES2/gl2.h>
#include <SOIL/SOIL.h>

using namespace std;

Application *Application::_instance = NULL;

Application::Application()
{

}

bool Application::init(int argc, char **argv)
{

	glutInit(&argc, argv);

	//Simple buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
	// glutInitWindowPosition(150, 125);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutCreateWindow("OpenGL");

	return true;
}

int Application::run()
{
	//Call to the drawing function
	glutTimerFunc(10, &Application::timerStatic, 1);
	glutDisplayFunc(&Application::drawStatic);
	glutMainLoop();
	return 0;
}

void Application::drawStatic()
{
	Application::getInstance()->draw();
}

void Application::timerStatic(int te)
{
	glutPostRedisplay(); // Redraw screen with new object data.
	glutTimerFunc(100, &Application::timerStatic, 1);
}

void Application::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);

	//Background color
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw order
	glutSwapBuffers();
}

