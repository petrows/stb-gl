#ifndef STBGL_GLOBAL_H
#define STBGL_GLOBAL_H

// Common C headers
#include <cstdint>

// C++ headers
#include <map>
#include <sstream>
#include <iostream>
#include <exception>
#include <memory>
#include <vector>

// Open GL ES
#include <GLES2/gl2.h>

namespace stbgl {

typedef std::vector<std::uint8_t> buffer_t;

typedef GLuint	shader_id_t;
typedef GLuint	shader_program_id_t;
typedef GLint	shader_attrib_id_t;
typedef GLuint	texture_id_t;
typedef GLuint	framebuffer_id_t;

}

#endif // STBGL_GLOBAL_H
