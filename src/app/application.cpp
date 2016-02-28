#include "application.h"

#include <string>
#include <GLES2/gl2.h>

using namespace std;
using namespace stbgl;

Application *Application::_instance = NULL;

bool Application::init(int argc, char **argv)
{
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	// _font = new font_t("font.ttf", 20);

	_ui_surface = surface_t::create(UI_W, UI_H);

	_test_surface_1 = surface_t::create("ts.png");

	_test_surface_2 = surface_t::create(512, 256);
	_test_surface_2->clear(0xFF00FF33);
	_test_surface_2->fill_rect(0x00FFFFFF, 10, 20, 40, 80);
	_test_surface_2->blit(_test_surface_1, 0, 0, _test_surface_1->width()/10, _test_surface_1->height()/10);
	//_test_surface_2->load_image("b.png");

	_test_surface_3 = surface_t::create("b2.png");

	_bg_texture = image_t::create("bg.png");

	return true;
}

void Application::draw()
{
	_ui_surface->clear(0x00000000);
	_ui_surface->set_current();

	drawing_t draw(UI_W, UI_H);
	blitting_t tex(UI_W, UI_H);

	//draw.set_color(color_t(0x888800FF));
	//draw.draw_rectangle(0, 0, SCREEN_W, SCREEN_H);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	tex.draw(_bg_texture, 0, 0, UI_W, UI_H);

	// Horz lines
	draw.set_color(color_t(0x66666666));
	for (uint32_t y=0; y<UI_H; y+=64)
	{
		draw.draw_rectangle(0, y, UI_W, 1);
	}
	for (uint32_t x=0; x<UI_W; x+=64)
	{
		draw.draw_rectangle(x, 0, 1, UI_H);
	}

	tex.draw(_test_surface_2, 150, 150, _test_surface_2->width(), _test_surface_2->height());

	//util_t::set_clip_rect(SCREEN_W, SCREEN_H, 100, 100, 400, 400);

	tex.draw(_test_surface_1, 300, 300, _test_surface_1->width(), _test_surface_1->height());


	tex.draw(_test_surface_3, 768, 200, _test_surface_3->width(), _test_surface_3->height());
	//util_t::reset_clip_rect();

	draw.set_color(color_t(0xFF00FF33));
	draw.draw_rectangle(100, 100, 500, 500);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_W, SCREEN_H);

	//Background color
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blitting_t draw_main(SCREEN_W, SCREEN_H);
	draw_main.draw(_ui_surface, 0, 0, SCREEN_W, SCREEN_H);
}

