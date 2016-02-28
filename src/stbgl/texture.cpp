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
}

texture_t::~texture_t()
{
	if (0 != _id) glDeleteTextures(1, &_id);
}

texture_ptr_t texture_t::create_from_image(const char *path)
{
	uint32_t w = 0, h = 0;
	return texture_ptr_t(new texture_t(w, h, 0));
}
