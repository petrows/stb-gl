#ifndef STBGL_SURFACE_H
#define STBGL_SURFACE_H

#include "global.h"
#include "texture.h"
#include "exception.h"
#include "color.h"

#include <vector>

namespace stbgl {

class surface_t;
typedef std::shared_ptr<surface_t> surface_ptr_t;

class surface_t {
private:
	surface_t(unsigned int w, unsigned int h, texture_ptr_t texture);

public:
	~surface_t();

	static surface_ptr_t create(unsigned int w, unsigned int h); //! Creates new surface object
	static surface_ptr_t create(texture_ptr_t texture);			 //! Creates new surface object using existing texture
	static surface_ptr_t create(const std::string &image_path);  //! Creates new surface object using image

	void clear(const color_t &color) { fill_rect(color, 0, 0, _width, _height); }						 //! Fill all surface with color
	surface_t &fill_rect(const color_t &color, int x, int y, unsigned int w, unsigned int h);			 //! Fill desired rectangle surface with color
	surface_t &blit(const surface_ptr_t &surface, int dst_x, int dst_y, unsigned int dst_w = 0, unsigned int dst_h = 0, int src_x = 0, int src_y = 0, unsigned int src_w = 0, unsigned int src_h = 0); //! Blit other surface to our
	surface_t &blit(const texture_ptr_t &texture, int dst_x, int dst_y, unsigned int dst_w = 0, unsigned int dst_h = 0, int src_x = 0, int src_y = 0, unsigned int src_w = 0, unsigned int src_h = 0); //! Blit other surface to our

	void memory_lock(buffer_t &data);
	void memory_unlock(buffer_t &data);

	texture_ptr_t texture() { return _texture; } //! get current framebuffer texture
	surface_t &set_current();					 //! Set this surface 'current' for drawing

	std::uint32_t width() { return _width; }
	std::uint32_t height() { return _height; }

	// Disable copy
	surface_t(const surface_t &) = delete;

private:
	texture_ptr_t _texture;
	framebuffer_id_t _framebuffer;
	const unsigned int _width, _height;
	shader_program_id_t _shaderProgram;

	shader_id_t _shaderVertexSolid;
	shader_id_t _shaderFragmentSolid;

	shader_attrib_id_t _shaderAttrPos;   // Quad vertex positions
	shader_attrib_id_t _shaderAttrColor; // Vertex fill color
};
}

#endif // STBGL_SURFACE_H
