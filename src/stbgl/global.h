#ifndef STBGL_GLOBAL_H
#define STBGL_GLOBAL_H

// Common C headers
#include <stdint.h>

// C++ headers
#include <map>
#include <sstream>
#include <iostream>
#include <exception>
#include <memory>

// Open GL ES
#include <GLES2/gl2.h>

namespace stbgl {

typedef GLuint shader_id_t;
typedef GLuint texture_id_t;
typedef GLuint framebuffer_id_t;

}

#endif // STBGL_GLOBAL_H
