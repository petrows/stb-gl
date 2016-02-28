#ifndef STBGL_IMAGE_T_H
#define STBGL_IMAGE_T_H

#include "global.h"
#include "surface.h"

#define STBGL_ENABLE_PNG 1
#define STBGL_ENABLE_JPG 1

#if defined(STBGL_ENABLE_PNG)
#	include <png.h>
#endif

namespace stbgl {

class image_t
{
public:
	image_t() = delete; // No constructor

	static texture_id_t create_texture(const std::string &path, uint32_t &w, uint32_t &h);
	static texture_id_t create_texture(const uint8_t *data, size_t size, uint32_t &w, uint32_t &h);

	static bool detect_image_png(const uint8_t *data, const size_t size);
	static bool detect_image_jpg(const uint8_t *data, const size_t size);

private:

#if defined(STBGL_ENABLE_PNG)
	class png_reader_t {
	private:
		struct _png_reader_memory_state
		{
			const unsigned char *buffer;
			png_uint_32 bufsize;
			png_uint_32 current_pos;
		}  memory_reader_state;
		png_structp _png_ptr;
		png_infop _info_ptr;
		int _bit_depth, _channels, _color_type, _interlace_type;
		int _width, _height;
		static void read_data_memory(png_structp png_ptr, png_bytep data, png_uint_32 length)
		{
			_png_reader_memory_state *f = (_png_reader_memory_state *)png_get_io_ptr(png_ptr);
			if (length > (f->bufsize - f->current_pos)) png_error(png_ptr, "read error in read_data_memory (loadpng)");
			memcpy(data, f->buffer + f->current_pos, length);
			f->current_pos += length;
		}
		void load_info();
	public:
		png_reader_t();
		~png_reader_t();
		texture_id_t load_png(const uint8_t *data, const size_t size, uint32_t &width, uint32_t &height);
	};
#endif

};

}

#endif // STBGL_IMAGE_T_H
