#include "drawing.h"
#include "exception.h"
#include "shader.h"
#include "util.h"

#include <iostream>

using namespace std;
using namespace stbgl;

GLuint drawing_t::_shader_program = 0;
GLint  drawing_t::_shader_attr_pos = 0;
GLint  drawing_t::_shader_attr_color = 0;

drawing_t::drawing_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
{
	if (0 == _shader_program)
	{
		prepare_shader();
	}
}

void drawing_t::clear(const color_t &color)
{
	set_color(color);
	draw_rectangle(0, 0, _w, _h);
}

void drawing_t::draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	// Draw points
	float triangleVertices[12];
	util_t::coord_rect(_w, _h, x, y, w, h, triangleVertices);

	const float triangleColors[] =
	{
		_color.r(), _color.g(), _color.b(), _color.a(),
		_color.r(), _color.g(), _color.b(), _color.a(),
		_color.r(), _color.g(), _color.b(), _color.a(),
		_color.r(), _color.g(), _color.b(), _color.a(),
	};

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(_shader_attr_pos);

	glVertexAttribPointer(_shader_attr_color, 4, GL_FLOAT, GL_FALSE, 0, triangleColors);
	glEnableVertexAttribArray(_shader_attr_color);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool drawing_t::prepare_shader()
{
	_shader_program = glCreateProgram();

	_shader_fragment = shader_t::load_shader_data(shader_t::get_solid_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex   = shader_t::load_shader_data(shader_t::get_solid_vert(), GL_VERTEX_SHADER);

	glAttachShader(_shader_program, _shader_fragment);
	glAttachShader(_shader_program, _shader_vertex);
	glLinkProgram(_shader_program);

	if (0 == _shader_program)
	{
		throw shader_error_t("Error create shader program: " + glGetError());
	}
	_shader_attr_pos = glGetAttribLocation(_shader_program, "a_v4Position");
	if (0 > _shader_attr_pos)
	{
		throw shader_error_t("Error create shader pos: " + glGetError());
	}
	_shader_attr_color = glGetAttribLocation(_shader_program, "a_v4FillColor");
	if (0 > _shader_attr_color)
	{
		throw shader_error_t("Error create shader color: " + glGetError());
	}

	return true;
}
