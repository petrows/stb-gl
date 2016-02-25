#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include <opengl-stb/stbgl_surface.h>

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
	const static uint32_t SCREEN_W = 1024;
	const static uint32_t SCREEN_H = 768;

	Application();
	bool init(int argc, char **argv);
	int run();

private:
	static void drawStatic();
	static void timerStatic(int te);
	void draw();

	stbgl_surface_t * _test_surface_1;
};

#endif // APPLICATION_H