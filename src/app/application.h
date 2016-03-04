#ifndef APPLICATION_H
#define APPLICATION_H

#include <stbgl/stbgl.h>

class Application {
private:
	static Application *_instance;

public:
	static Application *getInstance() {
		if (NULL != _instance)
			return _instance;
		_instance = new Application();
		return _instance;
	}

	void set_screen(std::uint32_t w, std::uint32_t h);
	std::uint32_t screen_width() { return _screen_w; }
	std::uint32_t screen_height() { return _screen_h; }

public:
	const static std::uint32_t UI_W = 1024;
	const static std::uint32_t UI_H = 768;

	Application();
	bool init();
	void draw();

private:
	std::uint32_t _screen_w;
	std::uint32_t _screen_h;

	stbgl::surface_ptr_t _ui_surface;

	stbgl::surface_ptr_t _test_surface_1;
	stbgl::surface_ptr_t _test_surface_2;
	stbgl::surface_ptr_t _test_surface_3;
	stbgl::font_ptr_t _font;

	stbgl::texture_ptr_t _bg_texture;
};

#endif // APPLICATION_H
