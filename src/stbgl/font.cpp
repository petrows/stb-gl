#include "font.h"
#include "shader.h"
#include "util.h"

#include "utf8/utf8.h"

using namespace stbgl;
using namespace std;

FT_Library font_t::_ft = nullptr;
shader_program_id_t font_t::_shader_program = 0;
shader_attrib_id_t font_t::_shader_attr_pos = 0;
shader_attrib_id_t font_t::_shader_attr_color = 0;
shader_attrib_id_t font_t::_shader_tex_uniform = 0;
shader_attrib_id_t font_t::_shader_tex_pos = 0;

font_t::font_t(const string &path, unsigned int size) : _size(size) {
	if (0 == _shader_program) {
		prepare_shader();
	}

	if (FT_New_Face(_ft, path.c_str(), 0, &_ft_face))
		throw font_error_t("Failed to load font " + path);

	FT_Set_Pixel_Sizes(_ft_face, 0, size);
}

font_ptr_t font_t::create(const string &path, unsigned int size) { return font_ptr_t(new font_t(path, size)); }

void font_t::draw(surface_ptr_t surface, const string &text_utf8, int x, int y) {
	if (!utf8::is_valid(text_utf8.begin(), text_utf8.end()))
		throw font_error_t("splitText: text is invalid utf8");

	string::const_iterator it = text_utf8.begin();
	FT_ULong slot_id;
	while (it != text_utf8.end()) {
		slot_id = utf8::unchecked::next(it);
		_glyth_t *glyth = render(slot_id);
		draw(surface, glyth, x, y);
		cout << "Slot: " << slot_id << endl;
		x += glyth->_advance;
	}
}

void font_t::draw(surface_ptr_t surface, _glyth_t *glyth, int x, int y) {
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(_shader_program);

	glBindTexture(GL_TEXTURE_2D, glyth->_texture);

	glUniform1i(_shader_tex_uniform, 0);

	static const float texture_coords[] = {
		1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0,
	};

	glVertexAttribPointer(_shader_tex_pos, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);
	glEnableVertexAttribArray(_shader_tex_pos);

	// Draw surfaces
	float triangleVertices[12];
	util_t::coord_rect(surface->width(), surface->height(), x, y + _size - glyth->_bitmap_top, glyth->_bitmap_width, glyth->_bitmap_height, triangleVertices);

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

font_t::_glyth_t *font_t::render(uint32_t char_utf8) {
	// Cached?
	auto cahe_it = _cache.find(char_utf8);
	if (_cache.end() != cahe_it) {
		cout << "Using cache for " << char_utf8 << endl;
		return cahe_it->second;
	}

	if (FT_Load_Char(_ft_face, char_utf8, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
		throw font_error_t("Failed to load Glyph");

	_glyth_t *glyth = new _glyth_t(_ft_face->glyph);

	_cache[char_utf8] = glyth;

	// Now remove old glyths from cache

	return glyth;
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

font_t::_glyth_t::_glyth_t(FT_GlyphSlot slot) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(_shader_program);

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, slot->bitmap.width, slot->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_bitmap_width = slot->bitmap.width;
	_bitmap_height = slot->bitmap.rows;
	_bitmap_left = slot->metrics.horiBearingX / 64;
	_bitmap_top = slot->metrics.horiBearingY / 64;
	_advance = slot->metrics.horiAdvance / 64;
}

font_t::_glyth_t::~_glyth_t() { glDeleteTextures(1, &_texture); }
