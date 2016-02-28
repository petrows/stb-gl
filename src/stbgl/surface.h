#ifndef STBGL_SURFACE_H
#define STBGL_SURFACE_H

#include "global.h"
#include "exception.h"

namespace stbgl {

class surface_t;
typedef std::shared_ptr<surface_t> surface_ptr_t;

class surface_t
{
private:
	surface_t(std::uint32_t w, std::uint32_t h, GLuint texture);
public:
	~surface_t();

	static surface_ptr_t create(std::uint32_t w, std::uint32_t h); //! Creates new surface object
	static surface_ptr_t create(std::uint32_t w, std::uint32_t h, GLuint &texture); //! Creates new surface object using existing texture
	static surface_ptr_t create_from_image(const char *path); //! Creates new surface object using image

	void clear(std::uint32_t color_rgba) { fill_rect(color_rgba, 0, 0, _width, _height); } //! Fill all surface with color
	surface_t &fill_rect(std::uint32_t color_rgba, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h); //! Fill desired rectangle surface with color
	surface_t &blit(surface_ptr_t &surface, std::uint32_t x, std::uint32_t y, std::uint32_t w = 0, std::uint32_t h = 0); //! Blit other surface to our
	surface_t &blit(GLuint texture, std::uint32_t x, std::uint32_t y, std::uint32_t w = 0, std::uint32_t h = 0); //! Blit other surface to our

	GLuint get_texture() { return _texture; } //! get current framebuffer texture
	surface_t &set_current(); //! Set this surface 'current' for drawing

	std::uint32_t width() { return _width; }
	std::uint32_t height() { return _height; }

	// Disable copy
	surface_t(const surface_t&) = delete;
private:
	GLuint _texture;
	GLuint _framebuffer;
	const std::uint32_t _width, _height;
	GLuint _shaderProgram;

	GLuint _shaderVertexSolid;
	GLuint _shaderFragmentSolid;

	GLint _shaderAttrPos; //Quad vertex positions
	GLint _shaderAttrColor; //Vertex fill color
};

}

#endif // STBGL_SURFACE_H
