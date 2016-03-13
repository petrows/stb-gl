#include "util.h"
#include <iostream>

using namespace std;
using namespace stbgl;

float util_t::coord_x(unsigned int w, int x) { return ((float)x - (w / 2)) / (float)(w / 2); }

float util_t::coord_y(unsigned int h, int y) { return ((h / 2) - (float)y) / (float)(h / 2); }

verticles_t util_t::coord_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h) {
	std::vector<float> vect;
	vect.resize(12);

	vect[0] = coord_x(screen_w, x + w);
	vect[1] = coord_y(screen_h, y);
	vect[2] = 0.0;

	vect[3] = coord_x(screen_w, x + w);
	vect[4] = coord_y(screen_h, y + h);
	vect[5] = 0.0;

	vect[6] = coord_x(screen_w, x);
	vect[7] = coord_y(screen_h, y + h);
	vect[8] = 0.0;

	vect[9] = coord_x(screen_w, x);
	vect[10] = coord_y(screen_h, y);
	vect[11] = 0.0;

	return vect;
}

verticles_t util_t::coord_texture(unsigned int size_w, unsigned int size_h, int x, int y, unsigned int w, unsigned int h) {
	/* texture mapping:
	 * c0,0 - b1,0
	 * d0,1 - a1,1
	 */
	std::vector<float> vect;
	vect.resize(8);

	if (0 == w) w = size_w - x;
	if (0 == h) h = size_h - y;

	float sz_x = size_w;
	float sz_y = size_h;

	float tex_pos_x = (float)x / sz_x;
	float tex_pos_y = (float)y / sz_y;

	// Size of texture.
	float tex_size_x = (float)w / sz_x;
	float tex_size_y = (float)h / sz_y;

	// Check the max size, should me inside the borders
	tex_size_x = std::min(tex_size_x, (1.0f - tex_pos_x));
	tex_size_y = std::min(tex_size_y, (1.0f - tex_pos_y));

	float pos_x1 = tex_pos_x;
	float pos_x2 = tex_pos_x + tex_size_x;

	float pos_y1 =  1.0f - (tex_pos_y + tex_size_y);
	float pos_y2 =  1.0f - (tex_pos_y);

	// a
	vect[0] = pos_x2;
	vect[1] = pos_y2;

	// b
	vect[2] = pos_x2;
	vect[3] = pos_y1;

	// c
	vect[4] = pos_x1;
	vect[5] = pos_y1;

	// d
	vect[6] = pos_x1;
	vect[7] = pos_y2;

	return vect;
}

void util_t::set_clip_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h) {
	(void)screen_w;
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, screen_h - y - h, w, h);
}

void util_t::reset_clip_rect() { glDisable(GL_SCISSOR_TEST); }
