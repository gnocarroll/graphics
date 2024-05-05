#ifndef GFX_UTIL_H
#define GFX_UTIL_H

/*
 *  Some additional utility functions and macros I am writing.
 */

#define sdl_perror(s) fprintf(stderr, "%s: %s\n", s, SDL_GetError())

#endif // GFX_UTIL_H
