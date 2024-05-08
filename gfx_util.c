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

/*
 *  Generate VBO with provided data.
 *   - vertices: data
 *   - n_vertices: number of vertices
 *   - n_attr: number of attributes per vertex
 *   - attr_lens: float array of length (at least) n_attr containing number of
 *     floats for each attribute
 *   - usage: usage arg for glBufferData()
 */

unsigned int get_VBO(float *vertices, size_t n_vertices, size_t n_attr,
                     size_t *attr_lens, GLenum usage) {
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  size_t vertex_size = 0;

  for (size_t i = 0; i < n_attr; i++) {
    vertex_size += attr_lens[i];
  }

  vertex_size *= sizeof(float);

  glBufferData(GL_ARRAY_BUFFER, n_vertices * vertex_size, vertices, usage);

  // will assume locations for attributes should begin at 0 and increase by
  // 1 for each successive attribute

  size_t cumulative_attr_len = 0;

  for (size_t i = 0; i < n_attr; i++) {
    glVertexAttribPointer(i, attr_lens[i], GL_FLOAT, GL_FALSE, vertex_size,
                          (void *) (cumulative_attr_len * sizeof(float)));
    glEnableVertexAttribArray(i);

    cumulative_attr_len += attr_lens[i];
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return VBO;
}

