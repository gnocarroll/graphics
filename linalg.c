#include "linalg.h"

#include <stdio.h>

void mat4_print(mat4 m) {
  printf("MAT4\n");

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.3f ", m.vec[i].data[j]);
    }

    printf("\n");
  }
}

