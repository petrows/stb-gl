#include "drawing.h"
#include "exception.h"
#include "shader.h"
#include "util.h"

#include <iostream>

using namespace std;
using namespace stbgl;

shader_program_id_t drawing_t::_shader_program = 0;
shader_attrib_id_t drawing_t::_shader_attr_pos = 0;
shader_attrib_id_t drawing_t::_shader_attr_color = 0;

drawing_t::drawing_t(unsigned int width, unsigned int height) : _w(width), _h(height) {
	if (0 == _shader_program) {
		prepare_shader();
	}
}

void drawing_t::clear(const color_t &color) {
	set_color(color);
	draw_rectangle(0, 0, _w, _h);
}

void drawing_t::draw_rectangle(int x, int y, unsigned int w, unsigned int h) {
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	// Draw points
	verticles_t triangleVertices = util_t::coord_rect(_w, _h, x, y, w, h);

	const float triangleColors[] = {
		_color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(),
	};

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, &triangleVertices.front());
	glEnableVertexAttribArray(_shader_attr_pos);

	glVertexAttribPointer(_shader_attr_color, 4, GL_FLOAT, GL_FALSE, 0, triangleColors);
	glEnableVertexAttribArray(_shader_attr_color);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawing_t::draw_line(int x1, int y1, int x2, int y2)
{
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	// Draw points
	float lineVertices[4] = {
		util_t::coord_x(_w, x1), util_t::coord_y(_h, y1),
		util_t::coord_x(_w, x2), util_t::coord_y(_h, y2)
	};

	const float lineColors[] = {
		_color.r(), _color.g(), _color.b(), _color.a(),
		_color.r(), _color.g(), _color.b(), _color.a()
	};

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 2, GL_FLOAT, GL_FALSE, 0, lineVertices);
	glEnableVertexAttribArray(_shader_attr_pos);

	glVertexAttribPointer(_shader_attr_color, 4, GL_FLOAT, GL_FALSE, 0, lineColors);
	glEnableVertexAttribArray(_shader_attr_color);

	glDrawArrays(GL_LINES, 0, 2);
}

bool drawing_t::prepare_shader() {
	_shader_program = glCreateProgram();

	_shader_fragment = shader_t::load_shader_data(shader_t::get_solid_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex = shader_t::load_shader_data(shader_t::get_solid_vert(), GL_VERTEX_SHADER);

	glAttachShader(_shader_program, _shader_fragment);
	glAttachShader(_shader_program, _shader_vertex);
	glLinkProgram(_shader_program);

	if (0 == _shader_program) {
		throw shader_error_t("Error create shader program: " + glGetError());
	}
	_shader_attr_pos = glGetAttribLocation(_shader_program, "a_v4Position");
	if (0 > _shader_attr_pos) {
		throw shader_error_t("Error create shader pos: " + glGetError());
	}
	_shader_attr_color = glGetAttribLocation(_shader_program, "a_v4FillColor");
	if (0 > _shader_attr_color) {
		throw shader_error_t("Error create shader color: " + glGetError());
	}

	return true;
}
