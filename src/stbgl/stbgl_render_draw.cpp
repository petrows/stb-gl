#include "stbgl_render_draw.h"
#include "stbgl_shader.h"
#include "stbgl_util.h"

#include <iostream>

using namespace std;

GLuint stbgl_render_draw_t::_shader_program = 0;
GLint  stbgl_render_draw_t::_shader_attr_pos = 0;
GLint  stbgl_render_draw_t::_shader_attr_color = 0;

stbgl_render_draw_t::stbgl_render_draw_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
{
	if (0 == _shader_program)
	{
		prepare_shader();
	}
}

void stbgl_render_draw_t::clear(const stbgl_color_t &color)
{
	set_color(color);
	draw_rectangle(0, 0, _w, _h);
}

void stbgl_render_draw_t::draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	// Draw points
	float triangleVertices[12];
	stbgl_util::coord_rect(_w, _h, x, y, w, h, triangleVertices);

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

bool stbgl_render_draw_t::prepare_shader()
{
	_shader_program = glCreateProgram();

	_shader_fragment = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex   = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_vert(), GL_VERTEX_SHADER);

	glAttachShader(_shader_program, _shader_fragment);
	glAttachShader(_shader_program, _shader_vertex);
	glLinkProgram(_shader_program);

	if (0 == _shader_program)
	{
		cerr << "Error create shader program! Error " << glGetError() << endl;
		return false;
	}

	_shader_attr_pos = glGetAttribLocation(_shader_program, "a_v4Position");
	if (0 > _shader_attr_pos)
	{
		cerr << "Shader pos error!" << endl;
		return false;
	}
	_shader_attr_color = glGetAttribLocation(_shader_program, "a_v4FillColor");
	if (0 > _shader_attr_color)
	{
		cerr << "Shader color error!" << endl;
		return false;
	}
	cout << "Prepared sold draw program" << endl;
	return true;
}
