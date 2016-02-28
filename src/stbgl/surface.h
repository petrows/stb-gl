#ifndef STBGL_SURFACE_H
#define STBGL_SURFACE_H

#include "global.h"
#include "texture.h"
#include "exception.h"

namespace stbgl {

class surface_t;
typedef std::shared_ptr<surface_t> surface_ptr_t;

class surface_t
{
private:
	surface_t(unsigned int w, unsigned int h);
public:
	~surface_t();

	static surface_ptr_t create(unsigned int w, unsigned int h); //! Creates new surface object
	static surface_ptr_t create(unsigned int w, unsigned int h, const texture_ptr_t &texture); //! Creates new surface object using existing texture
	static surface_ptr_t create(const std::string &&image_path); //! Creates new surface object using image

	void clear(std::uint32_t color_rgba) { fill_rect(color_rgba, 0, 0, _width, _height); } //! Fill all surface with color
	surface_t &fill_rect(std::uint32_t color_rgba, int x, int y, unsigned int w, unsigned int h); //! Fill desired rectangle surface with color
	surface_t &blit(const surface_ptr_t &surface, int x, int y, unsigned int w = 0, unsigned int h = 0); //! Blit other surface to our
	surface_t &blit(const texture_ptr_t &texture, int x, int y, unsigned int w = 0, unsigned int h = 0); //! Blit other surface to our

	texture_ptr_t texture() { return _texture; } //! get current framebuffer texture
	surface_t &set_current(); //! Set this surface 'current' for drawing

	std::uint32_t width() { return _width; }
	std::uint32_t height() { return _height; }

	// Disable copy
	surface_t(const surface_t&) = delete;
private:
	texture_ptr_t _texture;
	framebuffer_id_t _framebuffer;
	const unsigned int _width, _height;
	shader_program_id_t _shaderProgram;

	shader_id_t _shaderVertexSolid;
	shader_id_t _shaderFragmentSolid;

	shader_attrib_id_t _shaderAttrPos; //Quad vertex positions
	shader_attrib_id_t _shaderAttrColor; //Vertex fill color
};

}

#endif // STBGL_SURFACE_H
