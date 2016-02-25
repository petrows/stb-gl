#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include <stbgl/stbgl_surface.h>
#include <stbgl/stbgl_font.h>

class Surface;

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
	const static uint32_t SCREEN_W = 1920;
	const static uint32_t SCREEN_H = 1080;

	const static uint32_t UI_W = 1280;
	const static uint32_t UI_H = 720;

	Application();
	bool init(int argc, char **argv);
	int run();

private:
	static void drawStatic();
	static void timerStatic(int te);
	void draw();

	stbgl_surface_t * _ui_surface;

	stbgl_surface_t * _test_surface_1;
	stbgl_surface_t * _test_surface_2;
	stbgl_surface_t * _test_surface_3;
	stbgl_font_t * _font;

	GLuint _bg_texture;
};

#endif // APPLICATION_H
