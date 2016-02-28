#ifndef STBGL_RENDER_DRAW_H
#define STBGL_RENDER_DRAW_H

#include "global.h"
#include "color.h"

namespace stbgl {

class drawing_t
{
public:
	drawing_t(unsigned int width, unsigned int height);

	void clear(const color_t &color);
	void set_color(const color_t &color) { _color = color; }
	void draw_rectangle(int x, int y, unsigned int w, unsigned int h);
private:
	bool prepare_shader();
	const unsigned int _w, _h;
	color_t _color;

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static shader_program_id_t	_shader_program;
	static shader_attrib_id_t	_shader_attr_pos;
	static shader_attrib_id_t	_shader_attr_color;
};

}

#endif // STBGL_RENDER_DRAW_H
