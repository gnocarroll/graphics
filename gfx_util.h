#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include "gfx_config.h"

/*
 *  Some additional utility functions and macros I am writing.
 */

#if (SDL_DEBUG)
  #define sdl_perror(s) fprintf(stderr, "%s: %s\n", s, SDL_GetError())
#else
  // do nothing if debug output for SDL not desired
  #define sdl_perror(s) do {} while (0)
#endif

/*
 *  Returns (as a float) the number of seconds since the SDL library was
 *  initialized. So, essentially is the time the program has been running.
 */

#define GET_SECS() (((float) SDL_GetTicks64()) / 1000.0f)

/*
 *  time_elapsed will be the time between successive calls to
 *  update_time_elapsed().
 *
 *  update_time_elapsed(): updates static variable time_elapsed that is in
 *  gfx_util.c.
 *
 *  get_time_elapsed(): take a wild guess
 */

void update_time_elapsed(void);
float get_time_elapsed(void);

#endif // GFX_UTIL_H
