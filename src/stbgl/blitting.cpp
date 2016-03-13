#include "blitting.h"
#include "image.h"
#include "shader.h"
#include "util.h"

#include <png.h>

#include <iostream>

using namespace std;
using namespace stbgl;

shader_program_id_t blitting_t::_shader_program = 0;
shader_attrib_id_t blitting_t::_shader_attr_pos = 0;
shader_attrib_id_t blitting_t::_shader_tex_uniform = 0;
shader_attrib_id_t blitting_t::_shader_tex_pos = 0;

blitting_t::blitting_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
	, _src_x(0), _src_y(0)
	, _src_w(0), _src_h(0)
{
	if (0 == _shader_program) {
		prepare_shader();
	}
}

void blitting_t::set_src(int src_x, int src_y, unsigned int src_w, unsigned int src_h)
{
	_src_x = src_x;
	_src_y = src_y;
	_src_w = src_w;
	_src_h = src_h;
}

void blitting_t::draw(const texture_ptr_t &texture, int x, int y, unsigned int w, unsigned int h) {
	if (0 == w)
		w = texture->width();
	if (0 == h)
		h = texture->height();

	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->id());
	glUniform1i(_shader_tex_uniform, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (0 == _src_w) _src_w = texture->width();
	if (0 == _src_h) _src_h = texture->height();

	verticles_t texture_coords = util_t::coord_texture(texture->width(), texture->height(), _src_x, _src_y, _src_w, _src_h);

	glVertexAttribPointer(_shader_tex_pos, 2, GL_FLOAT, GL_FALSE, 0, &texture_coords.front());
	glEnableVertexAttribArray(_shader_tex_pos);

	// Draw surfaces
	verticles_t triangleVertices = util_t::coord_rect(_w, _h, x, y, w, h);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, &triangleVertices.front());
	glEnableVertexAttribArray(_shader_attr_pos);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool blitting_t::prepare_shader() {
	_shader_program = glCreateProgram();

	// Load shaders
	_shader_fragment = shader_t::load_shader_data(shader_t::get_texture_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex = shader_t::load_shader_data(shader_t::get_texture_vert(), GL_VERTEX_SHADER);

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
	_shader_tex_pos = glGetAttribLocation(_shader_program, "a_v2TextCoord");
	if (0 > _shader_tex_pos) {
		throw shader_error_t("Error create shader texture pos: " + glGetError());
	}
	_shader_tex_uniform = glGetUniformLocation(_shader_program, "Texture");
	if (0 > _shader_tex_uniform) {
		throw shader_error_t("Error create shader uniform: " + glGetError());
	}

	return true;
}
