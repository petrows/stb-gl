#include "application.h"

#include <string>
#include <GLES2/gl2.h>

using namespace std;
using namespace stbgl;

Application *Application::_instance = NULL;

Application::Application()
{
cout << "Init..." << endl;
}

bool Application::init(int argc, char **argv)
{

	cout << "Init..." << endl;
	glutInit(&argc, argv);

	//Simple buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
	// glutInitWindowPosition(150, 125);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutCreateWindow("OpenGL");

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	_font = new font_t("font.ttf", 20);

	_ui_surface = surface_t::create(UI_W, UI_H);

	_test_surface_1 = surface_t::create_from_image("b.png");

	_test_surface_2 = surface_t::create(512, 256);
	_test_surface_2->clear(0xFF00FF33);
	_test_surface_2->fill_rect(0x00FFFFFF, 10, 20, 40, 80);
	_test_surface_2->blit(_test_surface_1, 0, 0, _test_surface_2->width(), _test_surface_2->height());
	//_test_surface_2->load_image("b.png");

	_test_surface_3 = surface_t::create_from_image("b2.png");

	_bg_texture = texture_t::load_file_to_texture("bg.png");

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
	_ui_surface->clear(0x00000000);
	_ui_surface->set_current();

	drawing_t draw(UI_W, UI_H);
	texture_t tex(UI_W, UI_H);

	//draw.set_color(color_t(0x888800FF));
	//draw.draw_rectangle(0, 0, SCREEN_W, SCREEN_H);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	tex.draw(_bg_texture, 0, 0, UI_W, UI_H);

	// Horz lines
	draw.set_color(color_t(0x66666666));
	for (int y=0; y<UI_H; y+=64)
	{
		draw.draw_rectangle(0, y, UI_W, 1);
	}
	for (int x=0; x<UI_W; x+=64)
	{
		draw.draw_rectangle(x, 0, 1, UI_H);
	}

	tex.draw(_test_surface_2->get_texture(), 150, 150, _test_surface_2->width(), _test_surface_2->height());

	//util_t::set_clip_rect(SCREEN_W, SCREEN_H, 100, 100, 400, 400);

	tex.draw(_test_surface_1->get_texture(), 300, 300, 500, 500);


	tex.draw(_test_surface_3->get_texture(), 768, 200, _test_surface_3->width(), _test_surface_3->height());
	//util_t::reset_clip_rect();

	draw.set_color(color_t(0xFF00FF33));
	draw.draw_rectangle(100, 100, 500, 500);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);

	//Background color
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	texture_t draw_main(SCREEN_W, SCREEN_H);
	draw_main.draw(_ui_surface->get_texture(), 0, 0, SCREEN_W, SCREEN_H);

	//Draw order
	glutSwapBuffers();
}

