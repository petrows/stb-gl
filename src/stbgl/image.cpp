#include "image.h"

#include <cstring>
#include <iterator>
#include <fstream>
#include <vector>

using namespace std;
using namespace stbgl;

texture_ptr_t image_t::create(const string &path) {
	ifstream in_file(path, std::ios::binary);

	if (!in_file.is_open()) {
		throw image_error_t(string("Cant open file ") + path);
	}

	in_file >> std::noskipws;

	in_file.seekg(0, in_file.end);
	size_t in_size = in_file.tellg();
	in_file.seekg(0, in_file.beg);
	vector<uint8_t> data;
	data.reserve(in_size);

	// read the data:
	data.assign(istream_iterator<uint8_t>(in_file), istream_iterator<uint8_t>());

	texture_ptr_t ret = create(data.data(), data.size());
	cout << "Loaded image '" << path << "', size " << ret->width() << "x" << ret->height() << endl;
	return ret;
}

texture_ptr_t image_t::create(const uint8_t *data, size_t size) {
	unsigned int w, h;
// Try to detect file type now
#if defined(STBGL_ENABLE_PNG)
	if (detect_image_png(data, size)) {
		png_reader_t reader;
		// PNG image
		texture_id_t tex = reader.load_png(data, size, w, h);
		return texture_t::create(w, h, tex);
	}
#endif

	if (detect_image_jpg(data, size)) {
		// JPEG image
		// return load_jpg(data.data(), in_size, w, h);
	}

	// We here due to the unsopported format or error
	throw image_error_t(string("Unsopported image format for texture"));

	return 0;
}

bool image_t::detect_image_png(const uint8_t *data, const size_t size) {
	if (size < 4)
		return false;
	const uint8_t sig1[4] = {0x89, 0x50, 0x4E, 0x47};
	for (size_t x = 0; x < sizeof(sig1); x++) {
		if (sig1[x] != data[x]) {
			return false;
		}
	}
	return true;
}

bool image_t::detect_image_jpg(const uint8_t *data, const size_t size) {
	if (size < 4)
		return false;
	const uint8_t sig1[4] = {0xFF, 0xD8, 0xFF, 0xE0};
	for (size_t x = 0; x < sizeof(sig1); x++) {
		if (sig1[x] != data[x]) {
			return false;
		}
	}
	return true;
}

void image_t::png_reader_t::read_data_memory(png_structp png_ptr, png_bytep data, png_uint_32 length) {
	_png_reader_memory_state *f = (_png_reader_memory_state *)png_get_io_ptr(png_ptr);
	if (length > (f->bufsize - f->current_pos)) {
		throw image_error_t("Error at read_data_memory");
	}
	memcpy(data, f->buffer + f->current_pos, length);
	f->current_pos += length;
}

void image_t::png_reader_t::load_info() {
	_width = png_get_image_width(_png_ptr, _info_ptr);
	_height = png_get_image_height(_png_ptr, _info_ptr);
	_color_type = png_get_color_type(_png_ptr, _info_ptr);
	_bit_depth = png_get_bit_depth(_png_ptr, _info_ptr);
	_channels = png_get_channels(_png_ptr, _info_ptr);
}

image_t::png_reader_t::png_reader_t() {
	_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	_info_ptr = png_create_info_struct(_png_ptr);
}

image_t::png_reader_t::~png_reader_t() { png_destroy_read_struct(&_png_ptr, &_info_ptr, png_infopp_NULL); }

texture_id_t image_t::png_reader_t::load_png(const uint8_t *data, const size_t size, unsigned int &width, unsigned int &height) {
	memory_reader_state.buffer = (unsigned char *)data;
	memory_reader_state.bufsize = size;
	memory_reader_state.current_pos = 0;

	png_set_read_fn(_png_ptr, &memory_reader_state, (png_rw_ptr)read_data_memory);
	png_read_info(_png_ptr, _info_ptr);

	if (NULL == _info_ptr) {
		throw image_error_t("Error at png_read_info");
	}

	load_info();

	// Update info state

	if (_bit_depth == 16) {
		png_set_strip_16(_png_ptr);
		_bit_depth = 8;
	}

	if (png_get_valid(_png_ptr, _info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(_png_ptr);
	}

	switch (_color_type) {
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(_png_ptr);
		png_set_filler(_png_ptr, (png_byte)0xFF, PNG_FILLER_AFTER);
		break;
	case PNG_COLOR_TYPE_GRAY:
		if (_bit_depth < 8) {
			png_set_expand_gray_1_2_4_to_8(_png_ptr);
			_bit_depth = 8;
		}
		png_set_gray_to_rgb(_png_ptr);
		png_set_filler(_png_ptr, (png_byte)0xFF, PNG_FILLER_AFTER);
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		if (_bit_depth < 8) {
			png_set_expand_gray_1_2_4_to_8(_png_ptr);
			_bit_depth = 8;
		}
		png_set_gray_to_rgb(_png_ptr);
		break;
	case PNG_COLOR_TYPE_RGB:
		png_set_filler(_png_ptr, (png_byte)0xFF, PNG_FILLER_AFTER);
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		break;
	};

	png_read_update_info(_png_ptr, _info_ptr);
	load_info();

	// png error stuff, not sure libpng man suggests this.
	setjmp(png_jmpbuf(_png_ptr));

	// update width and height based on png info
	width = _width;
	height = _height;

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(_png_ptr, _info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	vector<png_byte> image_data;
	image_data.resize(rowbytes * height, 0x00);

	// row_pointers is for pointing to image_data for reading the png with libpng
	vector<png_bytep> row_pointers;
	row_pointers.resize(height, 0x00);

	// set the individual row_pointers to point at the correct offsets of image_data
	for (uint32_t i = 0; i < height; ++i)
		row_pointers[height - i - 1] = &(image_data[i * rowbytes]);

	// read the png into image_data through row_pointers
	png_read_image(_png_ptr, &(row_pointers[0]));

	// Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)&(image_data[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}
