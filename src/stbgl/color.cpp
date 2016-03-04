#include "color.h"

using namespace stbgl;

void color_t::set_color(float r, float g, float b, float a) {
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void color_t::set_color(uint32_t rgba) {
	float a = (uint8_t)(rgba & 0xff);
	float b = (uint8_t)((rgba & 0xff00) >> 8);
	float g = (uint8_t)((rgba & 0xff0000) >> 16);
	float r = (uint8_t)((rgba & 0xff000000) >> 24);

	set_color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
