#include "texture.h"
#include "shader.h"
#include "util.h"

#include <png.h>

#include <iostream>

using namespace std;
using namespace stbgl;

GLuint texture_t::_shader_program = 0;
GLint  texture_t::_shader_attr_pos = 0;
GLint  texture_t::_shader_tex_uniform = 0;
GLint  texture_t::_shader_tex_pos = 0;

texture_t::texture_t(uint32_t width, uint32_t height)
	: _w(width), _h(height)
{
	if (0 == _shader_program)
	{
		prepare_shader();
	}
}

void texture_t::draw(GLuint texture, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	glEnable(GL_BLEND);
	glUseProgram(_shader_program);
	glViewport(0, 0, _w, _h);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(_shader_tex_uniform, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	static const float texture_coords[] = {
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
	};

	glVertexAttribPointer(_shader_tex_pos, 2, GL_FLOAT, GL_FALSE, 0, texture_coords);
	glEnableVertexAttribArray(_shader_tex_pos);

	// Draw surfaces
	float triangleVertices[12];
	util_t::coord_rect(_w, _h, x, y, w, h, triangleVertices);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(_shader_attr_pos, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(_shader_attr_pos);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

GLuint texture_t::load_file_to_texture(const char *path, uint32_t &w, uint32_t &h)
{
	GLuint res = texture_png(path, w, h);

	if (!res)
	{
		cerr << "Error load texture " << path << endl;
	}

	cout << "Load texture " << path << ", size " << w << "x" << h << endl;

	return res;
}

GLuint texture_t::texture_png(const char *path, uint32_t &width, uint32_t &height)
{
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		return 0;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(fp);
		return 0;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
												 NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return (0);
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return (0);
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return (0);
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return (0);
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
				 NULL, NULL, NULL);

	bool has_alpha = (color_type & PNG_COLOR_MASK_ALPHA);
	int texture_format = has_alpha ? GL_RGBA : GL_RGB;

	//update width and height based on png info
	width = twidth;
	height = theight;

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return 0;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[height - i - 1] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, texture_format, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);

	return texture;
}

bool texture_t::prepare_shader()
{
	_shader_program = glCreateProgram();

	// Load shaders
	_shader_fragment = shader_t::load_shader_data(shader_t::get_texture_frag(), GL_FRAGMENT_SHADER);
	_shader_vertex   = shader_t::load_shader_data(shader_t::get_texture_vert(), GL_VERTEX_SHADER);

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
