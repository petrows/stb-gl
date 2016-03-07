#include "font.h"
#include "shader.h"
#include "util.h"

using namespace stbgl;

FT_Library font_t::_ft = nullptr;
shader_program_id_t font_t::_shader_program = 0;
shader_attrib_id_t font_t::_shader_attr_pos = 0;
shader_attrib_id_t font_t::_shader_attr_color = 0;
shader_attrib_id_t font_t::_shader_tex_uniform = 0;
shader_attrib_id_t font_t::_shader_tex_pos = 0;

font_t::font_t(const std::string &path, unsigned int size) : _size(size) {
	if (0 == _shader_program) {
		prepare_shader();
	}

	if (FT_New_Face(_ft, path.c_str(), 0, &_ft_face))
		throw font_error_t("Failed to load font " + path);

	FT_Set_Pixel_Sizes(_ft_face, 0, size);
}

font_ptr_t font_t::create(const std::string &path, unsigned int size) { return font_ptr_t(new font_t(path, size)); }

void font_t::draw(const std::string &text_utf8, int x, int y) {}

void font_t::draw(surface_ptr_t surface, const uint32_t char_utf8, int x, int y)
{
	_color.set_color(0xFFFFFFFF);

	if (FT_Load_Char(_ft_face, char_utf8, FT_LOAD_RENDER))
		throw font_error_t("Failed to load Glyph");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(_shader_program);

	GLuint _font_texture;
	glGenTextures(1, &_font_texture);
	glBindTexture(GL_TEXTURE_2D, _font_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _ft_face->glyph->bitmap.width, _ft_face->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _ft_face->glyph->bitmap.buffer);
	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(_shader_tex_uniform, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	static const float texture_coords[] = {
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
	};

	glVertexAttribPointer(_shader_tex_pos, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);
	glEnableVertexAttribArray(_shader_tex_pos);

	// Draw surfaces
	float triangleVertices[12];
	util_t::coord_rect(surface->width(), surface->height(), x, y, _ft_face->glyph->bitmap.width, _ft_face->glyph->bitmap.rows, triangleVertices);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(_shader_attr_pos);

	const float triangleColors[] = {
		_color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(), _color.r(), _color.g(), _color.b(), _color.a(),
	};

	glVertexAttribPointer(_shader_attr_color, 4, GL_FLOAT, GL_FALSE, 0, triangleColors);
	glEnableVertexAttribArray(_shader_attr_color);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool font_t::prepare_shader() {

	if (FT_Init_FreeType(&_ft))
		throw font_error_t("Could not init FreeType Library");

	_shader_program = glCreateProgram();

	// Load shaders
	shader_id_t _shader_fragment = shader_t::load_shader_data(shader_t::get_font_frag(), GL_FRAGMENT_SHADER);
	shader_id_t _shader_vertex = shader_t::load_shader_data(shader_t::get_font_vert(), GL_VERTEX_SHADER);

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
