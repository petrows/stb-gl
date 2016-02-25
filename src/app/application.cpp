#include "application.h"

#include <stbgl/stbgl_util.h>
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


	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	_font = new stbgl_font_t("font.ttf", 20);

	_test_surface_1 = new stbgl_surface_t(256, 256);
	_test_surface_1->load_image("b.png");

	_test_surface_2 = new stbgl_surface_t(256, 256);
	_test_surface_2->clear(0xFF00FF33);
	_test_surface_2->fill_rect(0x00FFFFFF, 10, 20, 40, 80);
	_test_surface_2->blit(_test_surface_1, 0, 0);
	//_test_surface_2->load_image("b.png");

	_test_surface_3 = stbgl_surface_t::from_image("b2.png");

	_bg_texture = stbgl_render_texture_t::load_file_to_texture("bg.jpg");

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
	stbgl_render_texture_t tex(SCREEN_W, SCREEN_H);

	//draw.set_color(stbgl_color_t(0x888800FF));
	//draw.draw_rectangle(0, 0, SCREEN_W, SCREEN_H);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	tex.draw(_bg_texture, 0, 0, SCREEN_W, SCREEN_H);

	// Horz lines
	draw.set_color(stbgl_color_t(0x66666666));
	for (int y=0; y<SCREEN_H; y+=64)
	{
		draw.draw_rectangle(0, y, SCREEN_W, 1);
	}
	for (int x=0; x<SCREEN_W; x+=64)
	{
		draw.draw_rectangle(x, 0, 1, SCREEN_H);
	}

	tex.draw(_test_surface_2->get_texture(), 150, 150, 256, 256);

	//stbgl_util_t::set_clip_rect(SCREEN_W, SCREEN_H, 100, 100, 400, 400);

	tex.draw(_test_surface_1->get_texture(), 300, 300, 500, 500);


	tex.draw(_test_surface_3->get_texture(), 768, 200, _test_surface_3->width(), _test_surface_3->height());
	//stbgl_util_t::reset_clip_rect();

	draw.set_color(stbgl_color_t(0xFF00FF33));
	draw.draw_rectangle(100, 100, 500, 500);

	//Draw order
	glutSwapBuffers();
}

