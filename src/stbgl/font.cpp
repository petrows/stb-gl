#include "font.h"
#include "shader.h"

using namespace stbgl;

shader_program_id_t font_t::_shader_program = 0;
shader_attrib_id_t font_t::_shader_attr_pos = 0;
shader_attrib_id_t font_t::_shader_attr_color = 0;
shader_attrib_id_t font_t::_shader_tex_uniform = 0;
shader_attrib_id_t font_t::_shader_tex_pos = 0;

font_t::font_t(const std::string &path, unsigned int size) : _size(size) {
	if (0 == _shader_program) {
		prepare_shader();
	}
}

font_ptr_t font_t::create(const std::string &path, unsigned int size) { return font_ptr_t(new font_t(path, size)); }

void font_t::draw(const std::string &text_utf8, int x, int y) {}

bool font_t::prepare_shader() {
	_shader_program = glCreateProgram();

	// Load shaders
	_shader_fragment = shader_t::load_shader_data(shader_t::get_font_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex = shader_t::load_shader_data(shader_t::get_font_vert(), GL_VERTEX_SHADER);

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
