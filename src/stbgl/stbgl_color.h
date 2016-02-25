#ifndef STBGL_COLOR_T_H
#define STBGL_COLOR_T_H

#include <stdint.h>

class stbgl_color_t
{
public:
	stbgl_color_t(uint32_t rgba = 0) { set_color(rgba); }
	stbgl_color_t(float r, float g, float b, float a)  { set_color(r, g, b, a); }

	void set_color(float r, float g, float b, float a);
	void set_color(uint32_t rgba);

	float r() { return _r; }
	float g() { return _g; }
	float b() { return _b; }
	float a() { return _a; }
private:
	float _r, _g, _b, _a;
};

#endif // STBGL_COLOR_T_H
