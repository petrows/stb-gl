#ifndef APPLICATION_H
#define APPLICATION_H

#include <stbgl/stbgl.h>

class Application
{
private:
	static Application *_instance;
public:
	static Application *getInstance() {
		if (NULL != _instance) return  _instance;
		_instance = new Application();
		return _instance;
	}

public:
	const static std::uint32_t SCREEN_W = 1920;
	const static std::uint32_t SCREEN_H = 1080;

	const static std::uint32_t UI_W = 1280;
	const static std::uint32_t UI_H = 720;

	Application() {}
	bool init();
	void draw();
private:
	stbgl::surface_ptr_t _ui_surface;

	stbgl::surface_ptr_t _test_surface_1;
	stbgl::surface_ptr_t _test_surface_2;
	stbgl::surface_ptr_t _test_surface_3;
	stbgl::font_ptr_t	 _font;

	stbgl::texture_ptr_t _bg_texture;
};

#endif // APPLICATION_H
