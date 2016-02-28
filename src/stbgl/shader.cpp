#include "shader.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace stbgl;

shader_id_t shader_t::load_shader_data(const char *data, int type)
{
	GLuint shader = glCreateShader(type);
	GLint compiled = 0;
	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen > 1)
		{
			char *infoLog = new char[(sizeof(char) * infoLen)];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			cerr << "Error compiling shader: " << infoLog << endl;
			delete[] infoLog;
		} else {
			cerr << "Unknown error compiling shader" << endl;
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

shader_id_t shader_t::load_shader_file(const char *filename, int type)
{
	ifstream shader_file(filename);

	string shader_data((istreambuf_iterator<char>(shader_file)), istreambuf_iterator<char>());

	return load_shader_data(shader_data.c_str(), type);
}

const char *shader_t::get_solid_frag()
{
	return " \
	varying vec4 v_v4FillColor;			\
	void main() {						\
		gl_FragColor = v_v4FillColor;	\
	}";
}

const char *shader_t::get_solid_vert()
{
	return " \
	attribute vec4 a_v4Position;		\
	attribute vec4 a_v4FillColor;		\
	varying vec4 v_v4FillColor;			\
	void main()	{						\
		v_v4FillColor = a_v4FillColor;	\
		gl_Position = a_v4Position;		\
	}";
}

const char *shader_t::get_texture_frag()
{
	return " \
	#ifndef GLES2						\n\
	#define lowp						\n\
	#else								\n\
	precision mediump float;			\n\
	#endif								\n\
	varying lowp vec2 TexCoordOut;		\
	uniform sampler2D Texture;			\
	void main() {						\
		  gl_FragColor = texture2D(Texture, TexCoordOut); \
	}";
}

const char *shader_t::get_texture_vert()
{
	return " \
	attribute vec4 a_v4Position;		\
	attribute vec4 a_v4FillColor;		\
	attribute vec2 a_v2TextCoord;		\
	varying vec4 v_v4FillColor;			\
	varying vec2 TexCoordOut;			\
	void main() {						\
		v_v4FillColor = a_v4FillColor;	\
		gl_Position = a_v4Position;		\
		TexCoordOut = a_v2TextCoord;	\
	}";
}
