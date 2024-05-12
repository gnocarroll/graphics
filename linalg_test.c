#include "linalg.h"

#include <stdio.h>

int main(void) {
  mat4_print(mat4_translate(mat4_identity(), VEC3(1.0f, 2.0f, 3.0f)));

  mat4_print(mat4_transpose(((mat4) { .data = {
      1, 2, 3, 4,
      0, 2, 3, 4,
      0, 0, 3, 4,
      0, 0, 0, 4
    } })
  ));

  return 0;
}

