#include "util.h"
#include <iostream>

using namespace std;
using namespace stbgl;

float util_t::coord_x(unsigned int w, int x)
{
	return ((float)x - (w/2))/ (float)(w/2);
}

float util_t::coord_y(unsigned int h, int y)
{
	return ((h/2) - (float)y) / (float)(h/2);
}

void util_t::coord_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h, float *vect)
{
	vect[0] = coord_x(screen_w, x+w);
	vect[1] = coord_y(screen_h, y);
	vect[2] = 0.0;

	vect[3] = coord_x(screen_w, x+w);
	vect[4] = coord_y(screen_h, y+h);
	vect[5] = 0.0;

	vect[6] = coord_x(screen_w, x);
	vect[7] = coord_y(screen_h, y+h);
	vect[8] = 0.0;

	vect[9] = coord_x(screen_w, x);
	vect[10] = coord_y(screen_h, y);
	vect[11] = 0.0;
}

void util_t::set_clip_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h)
{
	(void)screen_w;
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, screen_h - y - h, w, h);
}

void util_t::reset_clip_rect()
{
	glDisable(GL_SCISSOR_TEST);
}
