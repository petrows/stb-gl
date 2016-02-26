#include "exception.h"
#include "surface.h"
#include "shader.h"
#include "drawing.h"
#include "texture.h"

#include <iostream>

using namespace std;
using namespace stbgl;

surface_t::surface_t(uint32_t w, uint32_t h, GLuint texture)
	: _texture(texture)
	, _framebuffer(0)
	, _width(w)
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

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// std::stringstream ss; ss << "Wrong status: " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// throw framebuffer_error_t(ss.str().c_str());
	}
}

surface_t::~surface_t()
{
	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);
}

surface_ptr_t surface_t::create(uint32_t w, uint32_t h)
{
	return std::make_shared<surface_t>(w, h, 0);
}

surface_ptr_t surface_t::create(uint32_t w, uint32_t h, GLuint texture)
{
	return std::make_shared<surface_t>(w, h, texture);
}

surface_ptr_t surface_t::create_from_image(const char *path)
{
	uint32_t w, h;
	GLuint texture = texture_t::load_file_to_texture(path, w, h);
	return create(w, h, texture);
}

void surface_t::fill_rect(uint32_t color_rgba, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	set_current();

	drawing_t render(_width, _height);
	render.set_color(color_rgba);
	render.draw_rectangle(x, y, w, h);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer errir!!!" << endl;
	}
}

void surface_t::blit(surface_ptr_t &surface, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	if (!w) w = surface->width();
	if (!h) h = surface->width();
	set_current();
	texture_t tex(_width, _height);
	tex.draw(surface->get_texture(), x, y, w, h);
}

bool surface_t::set_current()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glViewport(0, 0, _width, _height);
}
