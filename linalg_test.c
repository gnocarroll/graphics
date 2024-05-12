#include "linalg.h"

#include <stdio.h>

int main(void) {
  mat4_print(mat4_mult(
    ((mat4) { .data = {  4,  3,  1, 0,
                      -5,  7,  2, 0,
                       3,  2,  6, 0,
                       0,  0,  0, 0 } }),
    ((mat4) { .data = {  1,  2,  2, 0,
                      -4,  3, -7, 0,
                       4,  6, -8, 0,
                       0,  0,  0, 0 } })
  ));

  return 0;
}

