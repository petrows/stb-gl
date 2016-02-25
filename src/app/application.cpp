#include "application.h"

#include <stbgl/stbgl_render_draw.h>
#include <stbgl/stbgl_render_texture.h>
#include <stbgl/stbgl_shader.h>
#include <stbgl/stbgl_surface.h>

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

	_test_surface_1 = new stbgl_surface_t(256, 256);
	_test_surface_1->load_image("b.png");

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
	glutTimerFunc(10, &Application::timerStatic, 1);
}

void Application::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);

	//Background color
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stbgl_render_draw_t draw(SCREEN_W, SCREEN_H);

	draw.set_color(stbgl_color_t(0x888800FF));
	draw.draw_rectangle(0, 0, SCREEN_W, SCREEN_H);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	stbgl_render_texture_t tex(SCREEN_W, SCREEN_H);
	tex.draw(_test_surface_1->get_texture(), 300, 300, 500, 500);

	draw.set_color(stbgl_color_t(0xFF00FF33));
	draw.draw_rectangle(0, 0, 700, 700);

	//Draw order
	glutSwapBuffers();
}

