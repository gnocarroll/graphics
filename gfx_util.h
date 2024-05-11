#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include <stddef.h>

#include <glad/glad.h>

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

/*
 *  vertices - data
 *  n_vertices - number of vertices
 *  n_attr - # attr / vert
 *  attr_lens - lengths for each attribute
 *  GLenum usage - usage parameter for glBufferData() 
 */

unsigned int get_VBO(float *vertices, size_t n_vertices, size_t n_attr,
                     size_t *attr_lens, GLenum usage);

/*
 *  indices - data
 *  n_indices - len
 *  usage - for glBufferData
 */

unsigned int get_EBO(unsigned int *indices, size_t n_indices, GLenum usage);

#endif // GFX_UTIL_H
