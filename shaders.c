#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

static char info_log[512];

void delete_shaders(unsigned int *shaders, size_t nmemb) {
  for (size_t i = 0; i < nmemb; i++) {
    glDeleteShader(shaders[i]);
    shaders[i] = 0;
  }
}

unsigned int get_program(unsigned int *shaders, size_t nmemb) {
  unsigned int program = glCreateProgram();

  if (!program) {
    return 0;
  }

  for (size_t i = 0; i < nmemb; i++) {
    glAttachShader(program, shaders[i]);
  }

  glLinkProgram(program);

  int success;

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info_log);
    fprintf(stderr, "creating shader program: %s\n", info_log);

    glDeleteProgram(program);

    return 0;
  }

  return program;
}

static unsigned int get_shader(const char *filename, GLenum shaderType) {
  unsigned int shader = glCreateShader(shaderType);

  if (shader == 0) {
    return 0;
  }

  int ret = compile_shader_file(shader, filename);

  if (ret == -1) {  // failure in compile_shader_file
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

unsigned int get_vertex_shader(const char *filename) {
  return get_shader(filename, GL_VERTEX_SHADER);
}

unsigned int get_frag_shader(const char *filename) {
  return get_shader(filename, GL_FRAGMENT_SHADER);
}

int compile_shader_file(unsigned int shader, const char *filename) {
  FILE *f = fopen(filename, "r");

  if (!f) {
    return -1;
  }

  long f_size;

  if ((fseek(f, 0, SEEK_END) == -1) || ((f_size = ftell(f)) == -1) ||
      (fseek(f, 0, SEEK_SET) == -1)) {
    perror("obtaining file size");
    fclose(f);
    return -1;
  }

  char *buff = malloc(f_size + 1);

  // "read" will indicate # bytes read from file

  size_t read = fread(buff, 1, f_size, f);
  fclose(f);

  if (read > f_size) {  // should be impossible
    perror("fread");
    exit(1);  // will exit because could have heap corruption
  }

  buff[read] = '\0';

  glShaderSource(shader, 1, &buff, NULL);
  glCompileShader(shader);

  free(buff);

  int success;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    fprintf(stderr, "shader compilation failed: %s\n", info_log);
    return -1;
  }

  return 0;
}

