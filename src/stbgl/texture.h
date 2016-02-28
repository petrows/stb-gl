#ifndef TEXTURE_T_H
#define TEXTURE_T_H

#include "global.h"

namespace stbgl {

class texture_t;
typedef std::shared_ptr<texture_t> texture_ptr_t;

class texture_t
{
protected:
	texture_t(std::uint32_t w, std::uint32_t h, texture_id_t texture);
public:
	~texture_t();

	static texture_ptr_t create(std::uint32_t w, std::uint32_t h, const texture_id_t texture = 0);

	GLuint get_id() { return _id; }

	// Disable copy
	texture_t(const texture_t&) = delete;
private:
	texture_id_t _id;
	const std::uint32_t _width, _height;
};

}

#endif // TEXTURE_T_H
