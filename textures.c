#include "textures.h"

#include <stdio.h>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
 *  Set preferred texture options.
 */

void set_texture_options(void) {
  // texture wrapping option

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // how to sample textures

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/*
 *  Generates 2D texture from filepath.
 */

unsigned int get_texture(const char *filepath) {
  int width, height, n_channels;
  unsigned char *data = stbi_load(filepath, &width, &height, &n_channels, 0);

  if (!data) {
    fprintf(stderr, "texture loading failed\n");
    return 0;
  }

  unsigned int texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  // glTexImage2D(target, mipmap level, format for storage, w, h, always 0,
  //              origin format, origin data type, data itself);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);  // OpenGL can generate us mipmap

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);

  return texture;
}

