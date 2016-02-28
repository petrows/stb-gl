#ifndef STBGL_FONT_T_H
#define STBGL_FONT_T_H

#include "global.h"
#include "surface.h"

#include <freetype2/ft2build.h>
#include <freetype2/ftstroke.h>
#include FT_FREETYPE_H

namespace stbgl {

class font_t;
typedef std::shared_ptr<font_t> font_ptr_t;

class font_t
{
private:
	font_t(const std::string &path, unsigned int size);
public:
	static font_ptr_t create(const std::string &path, unsigned int size);
	void draw(const std::string &text_utf8, int x, int y);

private:
	unsigned int _size;

	class _glyth_t
	{
	public:
		FT_Int _bitmap_top; // offset
		FT_Int _bitmap_left; // offset
		FT_Int _bitmap_width;
		FT_Int _bitmap_height;
		FT_Vector _advance;

		surface_ptr_t _surface;
		_glyth_t (FT_GlyphSlot slot);

		bool empty() {
			return _bitmap_height == 0 || _bitmap_width == 0;
		}

	};

	std::map< FT_ULong, _glyth_t > _cache;

	bool prepare_shader();

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static shader_program_id_t	_shader_program;
	static shader_attrib_id_t	_shader_attr_pos;
	static shader_attrib_id_t	_shader_attr_color;
	static shader_attrib_id_t	_shader_tex_uniform;
	static shader_attrib_id_t	_shader_tex_pos;
};

}

#endif // STBGL_FONT_T_H
