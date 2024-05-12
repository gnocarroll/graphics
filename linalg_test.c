#include "linalg.h"

#include <stdio.h>

int main(void) {
  mat4_print(mat4_translate(mat4_identity(), VEC3(1.0f, 2.0f, 3.0f)));

  return 0;
}

