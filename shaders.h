#ifndef SHADERS_H
#define SHADERS_H

#include <stddef.h>

/*
 *  Compile shader form specified file. 0 => error.
 */

unsigned int get_vertex_shader(const char *);
unsigned int get_frag_shader(const char *);

/*
 *  Provide already generated shader and filename.
 */

int compile_shader_file(unsigned int, const char *);

/*
 *  Provide array of already compiled shaders and length, this function will
 *  create and return a program of them combined.
 */

unsigned int get_program(unsigned int *, size_t);

/*
 *  Provide path to vertex shader, path to fragment shader and get shader
 *  program.
 */

unsigned int get_program_from_files(const char *, const char *);

// will call glDeleteShader() on all shaders in array and set their values to 0

void delete_shaders(unsigned int *, size_t);

#endif // SHADERS_H
