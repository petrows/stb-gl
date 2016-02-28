#ifndef STBGL_SHADER_H
#define STBGL_SHADER_H

#include "global.h"

namespace stbgl {

class shader_t
{
private:
	shader_t() {}
public:
	static shader_id_t load_shader_data(const char *data, int type);
	static shader_id_t load_shader_file(const std::string &filename, int type);

	static const char * get_solid_frag();
	static const char * get_solid_vert();

	static const char * get_texture_frag();
	static const char * get_texture_vert();

	static const char * get_font_frag();
	static const char * get_font_vert();
};

}

#endif // STBGL_SHADER_H
