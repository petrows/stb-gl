#ifndef STBGL_FONT_T_H
#define STBGL_FONT_T_H

#include <map>
#include <stdint.h>
#include <GLES2/gl2.h>

class stbgl_font_t
{
public:
	stbgl_font_t(const char *path, uint32_t size);

	void draw_string(const char *utf8, );

private:

	struct _glyth_t {
		GLuint		_texture;
		uint32_t	_key;
	};

	std::map< uint32_t, _glyth_t > _cache;
};

#endif // STBGL_FONT_T_H
