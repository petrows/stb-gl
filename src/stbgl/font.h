#ifndef STBGL_FONT_T_H
#define STBGL_FONT_T_H

#include "global.h"

namespace stbgl {

class font_t
{
public:
	font_t(const char *path, std::uint32_t size);

	void draw_string(const char *utf8, std::uint32_t x, std::uint32_t y);

private:

	struct _glyth_t {
		texture_id_t	_texture;
		std::uint32_t	_key;
	};

	std::map< std::uint32_t, _glyth_t > _cache;
};

}

#endif // STBGL_FONT_T_H
