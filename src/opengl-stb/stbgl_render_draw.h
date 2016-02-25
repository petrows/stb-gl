#ifndef STBGL_RENDER_DRAW_H
#define STBGL_RENDER_DRAW_H

#include <stdint.h>
#include <GLES2/gl2.h>

class stbgl_render_draw_t
{
public:
	stbgl_render_draw_t(uint32_t width, uint32_t height);
	void setBlendFlags(int color = GL_ONE, int alpha = GL_SRC_ALPHA);
	void setColor(float r, float g, float b, float a);
	void setColor(uint32_t rgba);
	void clear(uint32_t rgba);
	void drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

	float r() { return _r; }
	float g() { return _g; }
	float b() { return _b; }
	float a() { return _a; }
private:
	void prepareSaderSolid();
	const uint32_t _w, _h;
	float _r, _g, _b, _a;

	GLuint _shaderSolidFragment;
	GLuint _shaderSolidVertex;

	static GLuint	_shaderProgram;
	static GLint	_shaderAttrPos; //Quad vertex positions
	static GLint	_shaderAttrColor; //Vertex fill color
};

#endif // STBGL_RENDER_DRAW_H
