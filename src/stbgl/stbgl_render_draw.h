#ifndef STBGL_RENDER_DRAW_H
#define STBGL_RENDER_DRAW_H

#include <stdint.h>
#include <GLES2/gl2.h>

#include "stbgl_color.h"

class stbgl_render_draw_t
{
public:
	stbgl_render_draw_t(uint32_t width, uint32_t height);

	void clear(const stbgl_color_t &color);
	void set_color(const stbgl_color_t &color) { _color = color; }
	void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
private:
	bool prepare_shader();
	const uint32_t _w, _h;
	stbgl_color_t _color;

	GLuint _shader_fragment;
	GLuint _shader_vertex;

	static GLuint	_shader_program;
	static GLint	_shader_attr_pos; //Quad vertex positions
	static GLint	_shader_attr_color; //Vertex fill color
};

#endif // STBGL_RENDER_DRAW_H
