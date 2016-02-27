#ifndef STBGL_IMAGE_T_H
#define STBGL_IMAGE_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class image_t
{
public:
	static GLuint create_texture(const char *path);
private:
	image_t() {}
};

}

#endif // STBGL_IMAGE_T_H
