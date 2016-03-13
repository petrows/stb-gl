#ifndef STBGL_BLITTING_T_H
#define STBGL_BLITTING_T_H

#include "global.h"
#include "surface.h"

namespace stbgl {

class blitting_t {
public:
	blitting_t(std::uint32_t width, std::uint32_t height);

	void set_src(int src_x = 0, int src_y = 0, unsigned int src_w = 0, unsigned int src_h = 0); //! Set source area to blit

	void draw(const surface_ptr_t &surface, int x, int y, unsigned int w = 0, unsigned int h = 0) { return draw(surface->texture(), x, y, w, h); }
	void draw(const texture_ptr_t &texture, int x, int y, unsigned int w = 0, unsigned int h = 0);

private:
	bool prepare_shader();

	const unsigned int _w, _h;

	int _src_x, _src_y;
	unsigned int _src_w, _src_h;

	shader_id_t _shader_fragment;
	shader_id_t _shader_vertex;

	static shader_program_id_t _shader_program;
	static shader_attrib_id_t _shader_attr_pos; // Quad vertex positions
	static shader_attrib_id_t _shader_tex_uniform;
	static shader_attrib_id_t _shader_tex_pos; // Vertex fill color
};
}

#endif // STBGL_BLITTING_T_H
