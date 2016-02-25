#ifndef STBGL_RENDER_TEXTURE_T_H
#define STBGL_RENDER_TEXTURE_T_H

#include "stbgl_surface.h"

#include <GLES2/gl2.h>

class stbgl_render_texture_t
{
public:
	stbgl_render_texture_t(uint32_t width, uint32_t height);
	void draw(GLuint texture, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

	static GLuint load_file(stbgl_surface_t *surface, const char *path);
	static GLuint load_file_to_texture(const char *path, uint32_t &w, uint32_t &h);
private:
	bool prepare_shader();
	const uint32_t _w, _h;

	GLuint _shader_fragment;
	GLuint _shader_vertex;

	static GLuint	_shader_program;
	static GLint	_shader_attr_pos; //Quad vertex positions
	static GLint	_shader_tex_uniform;
	static GLint	_shader_tex_pos; //Vertex fill color
};

#endif // STBGL_RENDER_TEXTURE_T_H
