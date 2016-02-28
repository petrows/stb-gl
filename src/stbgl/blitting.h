#ifndef STBGL_BLITTING_T_H
#define STBGL_BLITTING_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class blitting_t
{
public:
	blitting_t(std::uint32_t width, std::uint32_t height);
	void draw(texture_id_t texture, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h);

	static GLuint load_file_to_texture(const char *path) { std::uint32_t w, h; return load_file_to_texture(path, w, h); }
	static GLuint load_file_to_texture(const char *path, std::uint32_t &w, std::uint32_t &h);
private:

	static GLuint texture_png(const char *path, std::uint32_t &width, std::uint32_t &height);

	bool prepare_shader();
	const std::uint32_t _w, _h;

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static GLuint	_shader_program;
	static GLint	_shader_attr_pos; //Quad vertex positions
	static GLint	_shader_tex_uniform;
	static GLint	_shader_tex_pos; //Vertex fill color
};

}

#endif // STBGL_BLITTING_T_H
