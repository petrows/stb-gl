#include "stbgl_util.h"

float stbgl_util_t::coord_x(uint32_t w, uint32_t x)
{
	return ((float)x - (w/2))/ (float)(w/2);
}

float stbgl_util_t::coord_y(uint32_t h, uint32_t y)
{
	return ((h/2) - (float)y) / (float)(h/2);
}

void stbgl_util_t::coord_rect(uint32_t screen_w, uint32_t screen_h, uint32_t x, uint32_t y, uint32_t w, uint32_t h, float *vect)
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

void stbgl_util_t::set_clip_rect(uint32_t screen_w, uint32_t screen_h, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	(void)screen_w;
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, screen_h - y - h, w, h);
}

void stbgl_util_t::reset_clip_rect()
{
	glDisable(GL_SCISSOR_TEST);
}
