#include "exception.h"
#include "surface.h"
#include "shader.h"
#include "image.h"
#include "drawing.h"
#include "blitting.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace stbgl;

surface_t::surface_t(unsigned int w, unsigned int h, texture_ptr_t texture)
	: _framebuffer(0)
	, _width(w)
	, _height(h)
{
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glEnable(GL_BLEND);

	if (nullptr == texture)
	{
		_texture = texture_t::create(w, h);
	} else {
		_texture = texture;
	}

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->id(), 0);

	if (nullptr == texture)
	{
		// Clear newbie texture
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw framebuffer_error_t(string("glCheckFramebufferStatus = ") + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	}
}

surface_t::~surface_t()
{
	glDeleteFramebuffers(1, &_framebuffer);
}

surface_ptr_t surface_t::create(unsigned int w, unsigned int h)
{
	return surface_ptr_t(new surface_t(w, h, nullptr));
}

surface_ptr_t surface_t::create(texture_ptr_t texture)
{
	return surface_ptr_t(new surface_t(texture->width(), texture->height(), texture));
}

surface_ptr_t surface_t::create(const string &image_path)
{
	return create(image_t::create(image_path));
}

surface_t& surface_t::fill_rect(const color_t &color, int x, int y, unsigned int w, unsigned int h)
{
	set_current();

	drawing_t render(_width, _height);
	render.set_color(color);
	render.draw_rectangle(x, y, w, h);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw framebuffer_error_t(string("glCheckFramebufferStatus = ") + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
	}

	return *this;
}

surface_t& surface_t::blit(const surface_ptr_t &surface, int x, int y, unsigned int w, unsigned int h)
{
	return blit(surface->texture(), x, y, w, h);
}

surface_t &surface_t::blit(const texture_ptr_t &texture, int x, int y, unsigned int w, unsigned int h)
{
	if (!w) w = texture->width();
	if (!h) h = texture->height();

	set_current();
	blitting_t tex(_width, _height);
	tex.draw(texture, x, y, w, h);

	return *this;
}

void surface_t::memory_lock(buffer_t &data)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	_texture->get_pixels(data);
}

void surface_t::memory_unlock(buffer_t &data)
{
	_texture->set_pixels(data);
}

surface_t& surface_t::set_current()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glBindTexture(GL_TEXTURE_2D, _texture->id());

	glViewport(0, 0, _width, _height);

	return *this;
}
