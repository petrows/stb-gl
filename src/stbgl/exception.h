#ifndef STBGL_EXCEPTION_H
#define STBGL_EXCEPTION_H

#include "global.h"

namespace stbgl {

class exception_t : public std::runtime_error {
public:
	explicit exception_t(const std::string &what) : std::runtime_error(what) {}
};

class shader_error_t : public exception_t {
public:
	explicit shader_error_t(const std::string &what) : exception_t(what) {}
};
class framebuffer_error_t : public exception_t {
public:
	explicit framebuffer_error_t(const std::string &what) : exception_t(what) {}
};
class texture_error_t : public exception_t {
public:
	explicit texture_error_t(const std::string &what) : exception_t(what) {}
};
class image_error_t : public exception_t {
public:
	explicit image_error_t(const std::string &what) : exception_t(what) {}
};
}

#endif // STBGL_EXCEPTION_H
