#include "texture.h"
#include "image.h"

#include <iostream>

using namespace std;
using namespace stbgl;

texture_t::texture_t(uint32_t w, uint32_t h, texture_id_t texture)
	: _id(texture)
	, _width(w)
	, _height(h)
{
	if (0 == _id)
	{
		glGenTextures(1, &_id);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, _id);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

texture_t::~texture_t()
{
	if (0 != _id) glDeleteTextures(1, &_id);
}

texture_ptr_t texture_t::create(uint32_t w, uint32_t h, const texture_id_t texture)
{
	return texture_ptr_t(new texture_t(w, h, texture));
}
