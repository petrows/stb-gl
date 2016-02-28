#ifndef STBGL_BLITTING_T_H
#define STBGL_BLITTING_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class blitting_t
{
public:
	blitting_t(uint32_t width, uint32_t height);
	void draw(texture_id_t texture, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

	static GLuint load_file_to_texture(const char *path) { uint32_t w, h; return load_file_to_texture(path, w, h); }
	static GLuint load_file_to_texture(const char *path, uint32_t &w, uint32_t &h);
private:

	static GLuint texture_png(const char *path, uint32_t &width, uint32_t &height);

	bool prepare_shader();
	const uint32_t _w, _h;

	GLuint _shader_fragment;
	GLuint _shader_vertex;

	static GLuint	_shader_program;
	static GLint	_shader_attr_pos; //Quad vertex positions
	static GLint	_shader_tex_uniform;
	static GLint	_shader_tex_pos; //Vertex fill color
};

}

#endif // STBGL_BLITTING_T_H
