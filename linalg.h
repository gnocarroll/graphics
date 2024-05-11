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

#define VEC2(a, b)       ((vec2) { .x = (a), .y = (b) })
#define VEC3(a, b, c)    ((vec3) { .x = (a), .y = (b), .z = (c) })
#define VEC4(a, b, c, d) ((vec4) { .x = (a), .y = (b), .z = (c), .w = (d) })

#define MULT_MEMBER(a) (arg1.a * arg2.a)

// inline functions
// dot product

inline float vec2_dot(vec2 arg1, vec2 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y);
}

inline float vec3_dot(vec3 arg1, vec3 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z);
}

inline float vec4_dot(vec4 arg1, vec4 arg2) {
  return MULT_MEMBER(x) + MULT_MEMBER(y) + MULT_MEMBER(z) + MULT_MEMBER(w);
}

// add

#define ADD_MEMBER(a) (arg1.a + arg2.a)

inline vec2 vec2_add(vec2 arg1, vec2 arg2) {
  return VEC2(ADD_MEMBER(x), ADD_MEMBER(y));
}

inline vec3 vec3_add(vec3 arg1, vec3 arg2) {
  return VEC3(ADD_MEMBER(x), ADD_MEMBER(y), ADD_MEMBER(z));
}

inline vec4 vec4_add(vec4 arg1, vec4 arg2) {
  return VEC4(ADD_MEMBER(x), ADD_MEMBER(y), ADD_MEMBER(z), ADD_MEMBER(w));
}

// cross product

#define CROSS_MEMBERS(a, b) (arg1.a * arg2.b - arg1.b * arg2.a)

inline vec3 vec3_cross(vec3 arg1, vec3 arg2) {
  return VEC3(CROSS_MEMBERS(y, x), CROSS_MEMBERS(z, x), CROSS_MEMBERS(x, y));
}



#endif // LINALG_H
