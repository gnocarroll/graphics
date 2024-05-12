#ifndef LINALG_H
#define LINALG_H

#include <stdint.h>
#include <math.h>

// CONSTANTS

// 20 decimal places for pi, single-precision float constant

#define PI (3.14159265358979323846f)

// some macros

// degrees to radians and vice versa

#define RADIANS(x) ((x) * PI / 180.0f)
#define DEGREES(x) ((x) * 180.0f / PI)

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
  return VEC4(f * v.x, f * v.y, f * v.z, f * v.w);
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
  return ((vec4) { .data = {
      vec4_dot(m.vec[0], v),
      vec4_dot(m.vec[1], v),
      vec4_dot(m.vec[2], v),
      vec4_dot(m.vec[3], v),
    }
  });
}

static inline mat4 mat4_mult(mat4 m1, mat4 m2) {
  mat4 m2_t = mat4_transpose(m2);

  return ((mat4) { .vec = {
      [0] = vec4_mult(m2_t, m1.vec[0]),
      [1] = vec4_mult(m2_t, m1.vec[1]),
      [2] = vec4_mult(m2_t, m1.vec[2]),
      [3] = vec4_mult(m2_t, m1.vec[3])
    }
  });
}

static inline mat4 mat4_translate(mat4 m, vec3 v) {
  m.vec[0] = vec4_add(m.vec[0], vec4_scale(v.data[0], m.vec[3]));
  m.vec[1] = vec4_add(m.vec[1], vec4_scale(v.data[1], m.vec[3]));
  m.vec[2] = vec4_add(m.vec[2], vec4_scale(v.data[2], m.vec[3]));

  return m;
}

static inline mat4 get_rotation_mat4(float radians, vec3 v) {
  float v_len = sqrtf(vec3_dot(v, v));

  v.x /= v_len;
  v.y /= v_len;
  v.z /= v_len;

  float sin_val = sinf(radians);
  float cos_val = cosf(radians);
  float one_minus_cos = 1.0f - cos_val;

  float x_y_prod = v.x * v.y * one_minus_cos;
  float x_z_prod = v.x * v.z * one_minus_cos;
  float y_z_prod = v.y * v.z * one_minus_cos;

  float x_sin_prod = v.x * sin_val;
  float y_sin_prod = v.y * sin_val;
  float z_sin_prod = v.z * sin_val;

  return ((mat4) { .vec = {
      [0].data = { cos_val + v.x * v.x * one_minus_cos, x_y_prod - z_sin_prod,
                   x_z_prod + y_sin_prod, 0.0f },
      [1].data = { x_y_prod + z_sin_prod, cos_val + v.y * v.y * one_minus_cos,
                   y_z_prod - x_sin_prod, 0.0f },
      [2].data = { x_z_prod - y_sin_prod, y_z_prod + x_sin_prod,
                   cos_val + v.z * v.z * one_minus_cos, 0.0f },
      [3].data = { 0.0f, 0.0f, 0.0f, 1.0f }
    }
  });
}

// matrix to apply it to, rotation amount in radians, axis of rotation

static inline mat4 mat4_rotate(mat4 m, float radians, vec3 v) {
  return mat4_mult(get_rotation_mat4(radians, v), m);
}

// v will contain scaling factors for x, y, z axis

static inline mat4 mat4_scale(mat4 m, vec3 v) {
  m.vec[0] = vec4_scale(v.data[0], m.vec[0]);
  m.vec[1] = vec4_scale(v.data[1], m.vec[1]);
  m.vec[2] = vec4_scale(v.data[2], m.vec[2]);

  return m;
}

#define translate(m, v) mat4_translate(m, v)
#define rotate(m, r, v) mat4_rotate(m, r, v)
#define scale(m, v)     mat4_scale(m, v)

#endif // LINALG_H
