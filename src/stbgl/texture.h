#ifndef TEXTURE_T_H
#define TEXTURE_T_H

#include "global.h"

namespace stbgl {

class texture_t;
typedef std::shared_ptr<texture_t> texture_ptr_t;

class texture_t
{
protected:
	texture_t(unsigned int w, unsigned int h, texture_id_t texture);
public:
	~texture_t();

	static texture_ptr_t create(unsigned int w, unsigned int h, const texture_id_t texture = 0);

	void get_pixels(buffer_t &data);
	void set_pixels(const buffer_t &data);
	void set_pixels(const std::uint8_t *data);

	texture_id_t id() { return _id; }

	unsigned int width() { return _width; }
	unsigned int height() { return _height; }

	// Disable copy
	texture_t(const texture_t&) = delete;
private:
	texture_id_t _id;
	const unsigned int _width, _height;
};

}

#endif // TEXTURE_T_H
