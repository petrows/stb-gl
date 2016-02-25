#include "stbgl_surface.h"
#include "stbgl_shader.h"
#include "stbgl_render_draw.h"

#include <iostream>

using namespace std;

stbgl_surface_t::stbgl_surface_t(int w, int h)
	: _width(w)
	, _height(h)
{
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	bindTexture();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	cout << "New surface, " << _width << "x" << _height << ", fb " << _framebuffer << ", tex " << _texture << endl;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer errir!!!" << endl;
	}
}

stbgl_surface_t::~stbgl_surface_t()
{

}

void stbgl_surface_t::fillRect(uint32_t colorRGBA, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	stbgl_render_draw_t render(_width, _height);
	render.setBlendFlags(GL_ONE, GL_ZERO);
	render.setColor(colorRGBA);
	render.drawRectangle(x, y, w, h);

	cout << "Draw rectangle on " << _texture << endl;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "Framebuffer errir!!!" << endl;
	}
}

void stbgl_surface_t::bindTexture()
{
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

	// glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

