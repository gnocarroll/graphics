#ifndef LINALG_H
#define LINALG_H

#include <stdint.h>
#include <math.h>

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

#define VEC2(a, b)       ((vec2) { .x = (a), .y = (b) })
#define VEC3(a, b, c)    ((vec3) { .x = (a), .y = (b), .z = (c) })
#define VEC4(a, b, c, d) ((vec4) { .x = (a), .y = (b), .z = (c), .w = (d) })

#define MULT_MEMBER(a) (arg1.a * arg2.a)

// inline functions
// dot product

static inline float vec2_dot(vec2 arg1, vec2 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y);
}

static inline float vec3_dot(vec3 arg1, vec3 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z);
}

static inline float vec4_dot(vec4 arg1, vec4 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z) + MULT_MEMBER(w);
}

// add

#define ADD_MEMBER(a) (arg1.a + arg2.a)

static inline vec2 vec2_add(vec2 arg1, vec2 arg2) {
  return VEC2(ADD_MEMBER(x), ADD_MEMBER(y));
}

static inline vec3 vec3_add(vec3 arg1, vec3 arg2) {
  return VEC3(ADD_MEMBER(x), ADD_MEMBER(y), ADD_MEMBER(z));
}

static inline vec4 vec4_add(vec4 arg1, vec4 arg2) {
  return VEC4(ADD_MEMBER(x), ADD_MEMBER(y), ADD_MEMBER(z), ADD_MEMBER(w));
}

static inline vec4 vec4_scale(float f, vec4 v) {
  for (uint8_t i = 0; i < 4; i++) {
    v.data[i] *= f;
  }

  return v;
}

// cross product

#define CROSS_MEMBERS(a, b) (arg1.a * arg2.b - arg1.b * arg2.a)

static inline vec3 vec3_cross(vec3 arg1, vec3 arg2) {
  return VEC3(CROSS_MEMBERS(y, x), CROSS_MEMBERS(z, x), CROSS_MEMBERS(x, y));
}

// matrices

typedef struct mat4 {
  union {
    float data[16];
    vec4 vec[4];
  };
} mat4;

void mat4_print(mat4);

static inline mat4 mat4_diag(float val) {
  return ((mat4) { .data = {
      val,  0.0f, 0.0f, 0.0f,
      0.0f, val,  0.0f, 0.0f,
      0.0f, 0.0f, val,  0.0f,
      0.0f, 0.0f, 0.0f, val
    }
  });
}

static inline mat4 mat4_identity(void) {
  return mat4_diag(1.0f);
}

static inline mat4 mat4_transpose(mat4 m) {
  return ((mat4) { .vec = {
      [0].data = m.vec[0].data[0], m.vec[1].data[0],
                 m.vec[2].data[0], m.vec[3].data[0],
      [1].data = m.vec[0].data[1], m.vec[1].data[1],
                 m.vec[2].data[1], m.vec[3].data[1],
      [2].data = m.vec[0].data[2], m.vec[1].data[2],
                 m.vec[2].data[2], m.vec[3].data[2],
      [3].data = m.vec[0].data[3], m.vec[1].data[3],
                 m.vec[2].data[3], m.vec[3].data[3],
    }
  });
}

#define mat4_t(m) mat4_transpose(m)

// various operations involving mat4

static inline vec4 vec4_mult(mat4 m, vec4 v) {
  return ((vec4) .data = {
    vec4_dot(m.vec[0], v),
    vec4_dot(m.vec[1], v),
    vec4_dot(m.vec[2], v),
    vec4_dot(m.vec[3], v),
  });
}

static inline mat4 mat4_translate(mat4 m, vec3 v) {
  for (uint8_t i = 0; i < 3; i++) {
    m.vec[i] = vec4_add(m.vec[i], vec4_scale(v.data[i], m.vec[3]));
  }

  return m;
}

#define translate(m, v) mat4_translate(m, v)

#endif // LINALG_H
