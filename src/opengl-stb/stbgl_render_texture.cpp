#include "stbgl_render_texture.h"
#include "stbgl_shader.h"
#include "stbgl_util.h"
#include "soil/SOIL.h"

#include <iostream>

using namespace std;

GLuint stbgl_render_texture_t::_shader_program = 0;
GLint  stbgl_render_texture_t::_shader_attr_pos = 0;
GLint  stbgl_render_texture_t::_shader_tex_uniform = 0;
GLint  stbgl_render_texture_t::_shader_tex_pos = 0;

stbgl_render_texture_t::stbgl_render_texture_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
{
	if (0 == _shader_program)
	{
		prepare_shader();
	}
}

void stbgl_render_texture_t::draw(GLuint texture, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(_shader_tex_uniform, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	static const float texture_coords[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
	};

	glVertexAttribPointer(_shader_tex_pos, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);
	glEnableVertexAttribArray(_shader_tex_pos);

	// Draw surfaces
	float triangleVertices[12];
	stbgl_util::coord_rect(_w, _h, x, y, w, h, triangleVertices);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(_shader_attr_pos);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

GLuint stbgl_render_texture_t::load_file(stbgl_surface_t *surface, const char *path)
{
	GLuint res = SOIL_load_OGL_texture
	(
		path,
		SOIL_LOAD_RGBA,
		surface->get_texture(),
		SOIL_FLAG_POWER_OF_TWO
	);

	return res;
}

bool stbgl_render_texture_t::prepare_shader()
{
	_shader_program = glCreateProgram();

	// Load shaders
	_shader_fragment = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex   = stbgl_shader_t::load_shader_data(stbgl_shader_t::get_solid_vert(), GL_VERTEX_SHADER);

	glAttachShader(_shader_program, _shader_fragment);
	glAttachShader(_shader_program, _shader_vertex);
	glLinkProgram(_shader_program);

	if (0 == _shader_program)
	{
		cerr << "Error create shader program! Error " << glGetError() << endl;
		return false;
	}

	_shader_attr_pos = glGetAttribLocation(_shader_program, "a_v4Position");
	if (0 > _shader_attr_pos)
	{
		cerr << "shader pos error!" << endl;
		return false;
	}
	_shader_tex_pos =glGetAttribLocation (_shader_program, "a_v2TextCoord");
	if (0 > _shader_tex_pos)
	{
		cerr << "Tex pos error!" << endl;
		return false;
	}
	_shader_tex_uniform = glGetUniformLocation(_shader_program, "Texture");
	if (0 > _shader_tex_uniform)
	{
		cout << "img_texture_uniform error!" << endl;
		return false;
	}

	return true;
}
