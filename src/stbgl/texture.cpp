#include "texture.h"

using namespace std;
using namespace stbgl;

texture_t::texture_t(uint32_t w, uint32_t h, GLuint texture)
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
	uint32_t w, h;
	return texture_ptr_t(new texture_t(w, h));
}
