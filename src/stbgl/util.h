#ifndef STBGL_UTIL_H
#define STBGL_UTIL_H

#include "global.h"

namespace stbgl {

class util_t {
public:
	/** Coordinates in PX from TOP-LEFT */
	static float coord_x(unsigned int w, int x);																					 //! From top-left zero point
	static float coord_y(unsigned int h, int y);																					 //! From top-left zero point
	static void coord_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h, float *vect); //! Generate triangle array for verticles, vect should be >= 12 size
	static void set_clip_rect(unsigned int screen_w, unsigned int screen_h, int x, int y, unsigned int w, unsigned int h);
	static void reset_clip_rect();
};
}

#endif // STBGL_UTIL_H
