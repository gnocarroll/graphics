#include "gfx_util.h"

#include <glad/glad.h>
#include <SDL.h>

static float time_elapsed;

/*
 *  Calculates time elapsed since the last time it was called and stores it in
 *  the (static) global variable time_elapsed. Do not call in multiple threads
 *  at the same time, just use it in one.
 */

void update_time_elapsed(void) {
  static float old_time = 0.0f;

  float new_time = GET_SECS();
  time_elapsed = new_time - old_time;

  old_time = new_time;
}

/*
 *  Returns time elapsed between successive calls to update_time_elapsed().
 */

float get_time_elapsed(void) {
  return time_elapsed;
}

