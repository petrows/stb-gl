#ifndef STBGL_SURFACE_H
#define STBGL_SURFACE_H

#include <stdint.h>
#include <GLES2/gl2.h>

class stbgl_surface_t
{
private:
	stbgl_surface_t(int w, int h);
	~stbgl_surface_t();

	void clear(uint32_t colorRGBA) { fillRect(colorRGBA, 0, 0, _width, _height); }
	void fillRect(uint32_t colorRGBA, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

	GLuint getTexture() { return _texture; }

private:

	void bindTexture();

	const uint32_t _width, _height;
	GLuint _framebuffer;
	GLuint _texture;
	GLuint _shaderProgram;

	GLuint _shaderVertexSolid;
	GLuint _shaderFragmentSolid;

	GLint _shaderAttrPos; //Quad vertex positions
	GLint _shaderAttrColor; //Vertex fill color
};

#endif // STBGL_SURFACE_H
