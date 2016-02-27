#include "exception.h"
#include "surface.h"
#include "shader.h"
#include "drawing.h"
#include "blitting.h"

#include <iostream>
#include <cstdlib>

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
	if (0 == _texture)
	{
		glGenTextures(1, &_texture);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, _texture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	} else {
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw framebuffer_error_t(string("glCheckFramebufferStatus = ") + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	}
}

surface_t::~surface_t()
{
	glDeleteTextures(1, &_texture);
	glDeleteFramebuffers(1, &_framebuffer);
}

surface_ptr_t surface_t::create(uint32_t w, uint32_t h)
{
	return surface_ptr_t(new surface_t(w, h, 0));
}

surface_ptr_t surface_t::create(uint32_t w, uint32_t h, GLuint &texture)
{
	surface_t * s = new surface_t(w, h, 0);
	s->set_current();
	blitting_t tex(w, h);
	tex.draw(texture, 0, 0, w, h);
	return surface_ptr_t(s);
}

surface_ptr_t surface_t::create_from_image(const char *path)
{
	uint32_t w, h;
	GLuint texture = blitting_t::load_file_to_texture(path, w, h);
	surface_ptr_t out = create(w, h, texture);
	glDeleteTextures(1, &texture);
	return out;
}

surface_t& surface_t::fill_rect(uint32_t color_rgba, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	set_current();

	drawing_t render(_width, _height);
	render.set_color(color_rgba);
	render.draw_rectangle(x, y, w, h);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw framebuffer_error_t(string("glCheckFramebufferStatus = ") + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	}

	return *this;
}

surface_t& surface_t::blit(surface_ptr_t &surface, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	if (!w) w = surface->width();
	if (!h) h = surface->width();

	return blit(surface->get_texture(), x, y, w, h);
}

surface_t &surface_t::blit(GLuint texture, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	set_current();
	blitting_t tex(_width, _height);
	tex.draw(texture, x, y, w, h);

	return *this;
}

surface_t& surface_t::set_current()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glViewport(0, 0, _width, _height);

	return *this;
}
