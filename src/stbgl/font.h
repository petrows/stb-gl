#ifndef STBGL_FONT_T_H
#define STBGL_FONT_T_H

#include "global.h"
#include "surface.h"

#include <list>
#include <ctime>

#include <freetype2/ft2build.h>
#include <freetype2/ftstroke.h>
#include FT_FREETYPE_H

namespace stbgl {

class font_t;
typedef std::shared_ptr<font_t> font_ptr_t;

class font_t {
	class _glyth_t;

private:
	font_t(const std::string &path, unsigned int size);

public:
	~font_t();

	static font_ptr_t create(const std::string &path, unsigned int size);
	void draw(surface_ptr_t surface, const std::string &text_utf8, int x, int y);
	void draw(surface_ptr_t surface, _glyth_t *glyth, int x, int y);

	void set_color(const color_t &color) { _color = color; }

	size_t get_string_size(const std::string &text_utf8);

	size_t get_max_height() const;

private:
	_glyth_t *render(std::uint32_t char_utf8);

	unsigned int _size;
	color_t _color;
	FT_Face _ft_face;

	class _glyth_t {
	public:
		FT_ULong _symbol;

		FT_Int _bitmap_top;  // offset
		FT_Int _bitmap_left; // offset
		FT_Int _bitmap_width;
		FT_Int _bitmap_height;
		FT_Int _advance;

		std::time_t _last_used;

		texture_id_t _texture;
		_glyth_t(FT_ULong symbol, FT_GlyphSlot slot);
		~_glyth_t();

		bool empty() { return _bitmap_height == 0 || _bitmap_width == 0; }
	};

	std::list<_glyth_t *> _cache;

	bool prepare_shader();

	static unsigned int _cache_max_size;
	static unsigned int _cache_min_size;

	static FT_Library _ft;

	static shader_program_id_t _shader_program;
	static shader_attrib_id_t _shader_attr_pos;
	static shader_attrib_id_t _shader_attr_color;
	static shader_attrib_id_t _shader_tex_uniform;
	static shader_attrib_id_t _shader_tex_pos;
};
}

#endif // STBGL_FONT_T_H
