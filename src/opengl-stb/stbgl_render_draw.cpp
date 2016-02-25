#include "stbgl_render_draw.h"
#include "stbgl_shader.h"
#include "stbgl_util.h"

#include <iostream>

using namespace std;

GLuint stbgl_render_draw_t::_shaderProgram = 0;
GLint  stbgl_render_draw_t::_shaderAttrPos = 0;
GLint  stbgl_render_draw_t::_shaderAttrColor = 0;

stbgl_render_draw_t::stbgl_render_draw_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
	, _r(0.0f), _g(0.0f), _b(0.0f), _a(1)
{
	if (0 == _shaderProgram)
	{
		prepareSaderSolid();
	}
	setBlendFlags();
}

void stbgl_render_draw_t::setBlendFlags(int color, int alpha)
{
	glEnable(GL_BLEND);
	glBlendFunc(color, alpha);
}

void stbgl_render_draw_t::setColor(float r, float g, float b, float a)
{
	 _r = r; _g = g; _b = b; _a = a;
	 cout << "New color: " << r << "," << g << "," << b << "," << a << endl;
}

void stbgl_render_draw_t::setColor(uint32_t rgba)
{
	float a = (uint8_t) (rgba & 0xff);
	float b = (uint8_t) ((rgba & 0xff00) >> 8);
	float g = (uint8_t) ((rgba & 0xff0000) >> 16);
	float r = (uint8_t) ((rgba & 0xff000000) >> 24);

	setColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void stbgl_render_draw_t::clear(uint32_t rgba)
{
	setColor(rgba);
	drawRectangle(0, 0, _w, _h);
}

void stbgl_render_draw_t::drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glUseProgram(_shaderProgram);
	glViewport(0, 0, _w, _h);

	// Draw points
	float triangleVertices[12];
	stbgl_util::coord_rect(_w, _h, x, y, w, h, triangleVertices);

	const float triangleColors[] =
	{
		_r, _g, _b, _a,
		_r, _g, _b, _a,
		_r, _g, _b, _a,
		_r, _g, _b, _a,
	};

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shaderAttrPos, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(_shaderAttrPos);

	glVertexAttribPointer(_shaderAttrColor, 4, GL_FLOAT, GL_FALSE, 0, triangleColors);
	glEnableVertexAttribArray(_shaderAttrColor);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void stbgl_render_draw_t::prepareSaderSolid()
{
	_shaderProgram = glCreateProgram();

	_shaderSolidFragment = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_frag(), GL_FRAGMENT_SHADER);
	_shaderSolidVertex   = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_vert(), GL_VERTEX_SHADER);

	glAttachShader(_shaderProgram, _shaderSolidFragment);
	glAttachShader(_shaderProgram, _shaderSolidVertex);
	glLinkProgram(_shaderProgram);

	if (0 == _shaderProgram)
	{
		cerr << "Error create shader program! Error " << glGetError() << endl;
		return;
	}

	_shaderAttrPos = glGetAttribLocation(_shaderProgram, "a_v4Position");
	if (0 > _shaderAttrPos)
	{
		cerr << "Shader pos error!" << endl;
		return;
	}
	_shaderAttrColor = glGetAttribLocation(_shaderProgram, "a_v4FillColor");
	if (0 > _shaderAttrColor)
	{
		cerr << "Shader color error!" << endl;
		return;
	}
	cout << "Prepared sold draw program" << endl;
}
