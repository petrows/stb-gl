#ifndef STBGL_SHADER_H
#define STBGL_SHADER_H

#include <GLES2/gl2.h>

class stbgl_shader_t
{
private:
	stbgl_shader_t() {}
public:
	static GLuint load_shader_data(const char *data, int type);
	static GLuint load_shader_file(const char *filename, int type);

	static const char * get_solid_frag();
	static const char * get_solid_vert();
	static const char * get_texture_frag();
	static const char * get_texture_vert();
};

#endif // STBGL_SHADER_H