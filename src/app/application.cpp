#include "application.h"

#include <string>
#include <GLES2/gl2.h>

#include <stbgl/exception.h>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace stbgl;

Application *Application::_instance = NULL;

void Application::set_screen(uint32_t w, uint32_t h) {
	_screen_w = w;
	_screen_h = h;
}

Application::Application() : _screen_w(1024), _screen_h(768) {}

bool Application::init() {
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);



	_ui_surface = surface_t::create(UI_W, UI_H);
	_ui_surface->clear(0x00000000);

	for (uint32_t y = 0; y < UI_H; y += 64) {
		_ui_surface->fill_rect(color_t(0x66666699), 0, y, UI_W, 1);
	}
	for (uint32_t x = 0; x < UI_W; x += 64) {
		_ui_surface->fill_rect(color_t(0x66666699), x, 0, 1, UI_H);
	}

	surface_ptr_t test_surface;

	test_surface = surface_t::create(0xFF, 0xFF);
	test_surface->clear(0xB2B2B2B2);

	buffer_t data;
	test_surface->memory_lock(data);
	for (unsigned int y = 0; y < test_surface->height(); y++) {
		for (unsigned int x = 0; x < test_surface->width(); x++) {
			uint8_t *ptr = &(data[((y * test_surface->width()) + x) * 4]);

			ptr[0] = 0xFF - (uint8_t)y; // R
			ptr[1] = (uint8_t)x;		// G
			ptr[2] = (uint8_t)y;		// B
			ptr[3] = 0xFF;				// A
		}
	}
	test_surface->memory_unlock(data);
	//_ui_surface->blit(test_surface, 0, 0);

	test_surface = surface_t::create("b2.png");

	//_ui_surface->blit(test_surface, 512, 100);

	//_test_surface_2 = surface_t::create(512, 256);
	//_test_surface_2->clear(0xFF00FF33);
	//_test_surface_2->fill_rect(0x00FFFFFF, 10, 20, 40, 80);
	//_test_surface_2->blit(_test_surface_1, 0, 0, _test_surface_1->width()/10, _test_surface_1->height()/10);
	//_test_surface_2->load_image("b.png");

	//_test_surface_3 = surface_t::create("b2.png");

	// _bg_texture = image_t::create("bg.png");

	/*
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		throw stbgl::exception_t("ERROR::FREETYPE: Could not init FreeType Library");



	if (FT_Load_Char(face, 'y', FT_LOAD_RENDER))
		throw stbgl::exception_t("ERROR::FREETYTPE: Failed to load Glyph");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	GLuint _font_texture;
	glGenTextures(1, &_font_texture);
	glBindTexture(GL_TEXTURE_2D, _font_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture_ptr_t font_tex = texture_t::create(face->glyph->bitmap.width, face->glyph->bitmap.rows, _font_texture);
	// font_tex->set_pixels(face->glyph->bitmap.buffer);

	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
	_ui_surface->blit(font_tex, 0, 0);*/

	_font = font_t::create("liberation-sans.ttf", 20);
	_font->set_color(color_t(0xFF0000FF));
	_ui_surface->set_current();
	_font->draw(_ui_surface, std::string("КАК СЧАСТЛИВ Я, КОГДА МОГУ ПОКИНУТЬ"), 50, 10);
	_font->draw(_ui_surface, std::string("Докучный шум столицы и двора"), 50, 30);
	_font->draw(_ui_surface, std::string("И убежать в пустынные дубровы"), 50, 50);

	return true;
}

void Application::draw() {
	// draw.set_color(color_t(0x888800FF));
	// draw.draw_rectangle(0, 0, SCREEN_W, SCREEN_H);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	// tex.draw(_bg_texture, 0, 0, UI_W, UI_H);

	// Horz lines

	// tex.draw(_test_surface_2, 150, 150, _test_surface_2->width(), _test_surface_2->height());

	// util_t::set_clip_rect(SCREEN_W, SCREEN_H, 100, 100, 400, 400);

	// tex.draw(_test_surface_1, 300, 300);

	// tex.draw(_test_surface_3, 768, 200, _test_surface_3->width(), _test_surface_3->height());
	// util_t::reset_clip_rect();

	// draw.set_color(color_t(0xFF00FF33));
	// draw.draw_rectangle(100, 100, 500, 500);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width(), screen_height());

	// Background color
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	blitting_t draw_main(screen_width(), screen_height());
	draw_main.draw(_ui_surface, 0, 0, screen_width(), screen_height());
}
