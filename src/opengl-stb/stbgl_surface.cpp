#include "stbgl_surface.h"
#include "stbgl_shader.h"
#include "stbgl_render_draw.h"

#include "soil/SOIL.h"

#include <iostream>

using namespace std;

stbgl_surface_t::stbgl_surface_t(uint32_t w, uint32_t h)
	: _width(w)
	, _height(h)
{
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glEnable(GL_BLEND);
	glGenTextures(1, &_texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _texture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	cout << "New surface, " << _width << "x" << _height << ", fb " << _framebuffer << ", tex " << _texture << endl;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer errir!!!" << endl;
	}
}

stbgl_surface_t::~stbgl_surface_t()
{
	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);
}

void stbgl_surface_t::fill_rect(uint32_t color_rgba, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	set_current();

	stbgl_render_draw_t render(_width, _height);
	render.set_color(color_rgba);
	render.draw_rectangle(x, y, w, h);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer errir!!!" << endl;
	}
}

bool stbgl_surface_t::set_current()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glViewport(0, 0, _width, _height);
}

bool stbgl_surface_t::load_image(const char *path)
{
	GLuint res = SOIL_load_OGL_texture
	(
		path,
		SOIL_LOAD_RGBA,
		_texture,
		SOIL_FLAG_POWER_OF_TWO
	);

	return res != 0;
}
