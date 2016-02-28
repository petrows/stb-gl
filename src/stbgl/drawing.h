#ifndef STBGL_RENDER_DRAW_H
#define STBGL_RENDER_DRAW_H

#include "global.h"
#include "color.h"

namespace stbgl {

class drawing_t
{
public:
	drawing_t(std::uint32_t width, std::uint32_t height);

	void clear(const color_t &color);
	void set_color(const color_t &color) { _color = color; }
	void draw_rectangle(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h);
private:
	bool prepare_shader();
	const std::uint32_t _w, _h;
	color_t _color;

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static GLuint	_shader_program;
	static GLint	_shader_attr_pos; //Quad vertex positions
	static GLint	_shader_attr_color; //Vertex fill color
};

}

#endif // STBGL_RENDER_DRAW_H
