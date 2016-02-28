#ifndef STBGL_FONT_T_H
#define STBGL_FONT_T_H

#include "global.h"

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

	struct _glyth_t {
		texture_id_t	_texture;
		std::uint32_t	_key;
	};

	std::map< std::uint32_t, _glyth_t > _cache;
};

}

#endif // STBGL_FONT_T_H
