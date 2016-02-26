#ifndef STBGL_UTIL_H
#define STBGL_UTIL_H

#include "global.h"

namespace stbgl {

class util_t
{
public:
	/** Coordinates in PX from TOP-LEFT */
	static float coord_x(uint32_t w, uint32_t x); //! From top-left zero point
	static float coord_y(uint32_t h, uint32_t y); //! From top-left zero point
	static void coord_rect(uint32_t screen_w, uint32_t screen_h, uint32_t x, uint32_t y, uint32_t w, uint32_t h, float *vect); //! Generate triangle array for verticles, vect should be >= 12 size

	static void set_clip_rect(uint32_t screen_w, uint32_t screen_h, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	static void reset_clip_rect();
};

}

#endif // STBGL_UTIL_H
