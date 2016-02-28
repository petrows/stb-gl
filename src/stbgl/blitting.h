#ifndef STBGL_BLITTING_T_H
#define STBGL_BLITTING_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class blitting_t
{
public:
	blitting_t(std::uint32_t width, std::uint32_t height);
	void draw(const surface_ptr_t &surface, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h) { return draw(surface->texture(), x, y, w, h); }
	void draw(const texture_ptr_t &texture, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h);

private:
	bool prepare_shader();

	const std::uint32_t _w, _h;

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static shader_program_id_t	_shader_program;
	static shader_attrib_id_t	_shader_attr_pos; //Quad vertex positions
	static shader_attrib_id_t	_shader_tex_uniform;
	static shader_attrib_id_t	_shader_tex_pos; //Vertex fill color
};

}

#endif // STBGL_BLITTING_T_H
