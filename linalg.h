#ifndef LINALG_H
#define LINALG_H

// VECTORS
// float vectors

typedef struct vec2 {
  union {
    struct {
      float x;
      float y;
    };

    float data[2];
  };
} vec2;

typedef struct vec3 {
  union {
    struct {
      float x;
      float y;
      float z;
    };

    float data[3];
  };
} vec3;

typedef struct vec4 {
  union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };

    float data[4];
  };
} vec4;

// create vector with given args

#define VEC2(a, b)       ((vec2) { .x = a, .y = b })
#define VEC3(a, b, c)    ((vec3) { .x = a, .y = b, .z = c })
#define VEC4(a, b, c, d) ((vec4) { .x = a, .y = b, .z = c, .w = d })

#define MULT_MEMBER(memb) (arg1.memb * arg2.memb)

// inline functions

inline float vec2_dot(vec2 arg1, vec2 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y);
}

inline float vec3_dot(vec3 arg1, vec3 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z);
}

inline float vec4_dot(vec4 arg1, vec4 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z) + MULT_MEMBER(w);
}

#endif // LINALG_H
