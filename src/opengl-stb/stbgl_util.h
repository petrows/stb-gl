#ifndef STBGL_UTIL_H
#define STBGL_UTIL_H

#include <stdint.h>

class stbgl_util
{
public:
	/** Coordinates in PX from TOP-LEFT */
	static float coord_x(uint32_t w, uint32_t x); //! From top-left zero point
	static float coord_y(uint32_t h, uint32_t y); //! From top-left zero point
	static void coord_rect(uint32_t screen_w, uint32_t screen_h, uint32_t x, uint32_t y, uint32_t w, uint32_t h, float *vect); //! Generate triangle array for verticles, vect should be >= 12 size
};

#endif // STBGL_UTIL_H
