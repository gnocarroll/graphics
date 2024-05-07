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

#endif // GFX_UTIL_H
