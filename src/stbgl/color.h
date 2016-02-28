#ifndef STBGL_COLOR_T_H
#define STBGL_COLOR_T_H

#include "global.h"

namespace stbgl {

class color_t
{
public:
	color_t(std::uint32_t rgba = 0) { set_color(rgba); }
	color_t(float r, float g, float b, float a)  { set_color(r, g, b, a); }

	void set_color(float r, float g, float b, float a);
	void set_color(std::uint32_t rgba);

	float r() { return _r; }
	float g() { return _g; }
	float b() { return _b; }
	float a() { return _a; }
private:
	float _r, _g, _b, _a;
};

}

#endif // STBGL_COLOR_T_H
