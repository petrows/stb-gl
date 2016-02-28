#ifndef STBGL_IMAGE_T_H
#define STBGL_IMAGE_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class image_t
{
public:
	static texture_id_t create_texture(const std::string &path);
private:
	image_t() {}
};

}

#endif // STBGL_IMAGE_T_H
