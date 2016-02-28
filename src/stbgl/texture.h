#ifndef TEXTURE_T_H
#define TEXTURE_T_H

#include "global.h"

namespace stbgl {

class texture_t;
typedef std::shared_ptr<texture_t> texture_ptr_t;

class texture_t
{
protected:
	texture_t(uint32_t w, uint32_t h, GLuint texture);
public:
	~texture_t();

	texture_ptr_t create_from_image(const char *path);

	GLuint get_id() { return _id; }

	// Disable copy
	texture_t(const texture_t&) = delete;
private:
	const uint32_t _width, _height;
	const GLuint _id;
};

}

#endif // TEXTURE_T_H
