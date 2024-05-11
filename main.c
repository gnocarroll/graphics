#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <SDL.h>

#include "gfx_config.h"
#include "gfx_util.h"
#include "event_handling.h"
#include "shaders.h"
#include "linalg.h"
#include "textures.h"

SDL_Window *sdl_setup(const char *);
void gl_debug_message(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *,
                      const void *);

int main(void) {
  // I will begin my going through LearnOpenGL.com and then once I have a
  // larger codebase I can move into some more graphics
  // experimentation/development

  // sdl_setup will register SDL_Quit with atexit

  SDL_Window *window = sdl_setup("LearnOpenGL");

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    fprintf(stderr, "GLAD failed\n");
    exit(1);
  }

#if (GL_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_debug_message, NULL);
#endif

  int w;
  int h;

  SDL_GetWindowSize(window, &w, &h);

  glViewport(0, 0, w, h);

  // relative mode hides cursor and constrains mouse position to window
  // would be used in FPS or something like that

  // SDL_SetRelativeMouseMode(SDL_TRUE);

  set_texture_options();

  float vertices[] = {
      // positions          // colors           // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int program = get_program_from_files("vertex.glsl", "frag.glsl");
  
  if (!program) {
    fprintf(stderr, "creating shader program failed\n");
    exit(1);
  }

  glUseProgram(program);

  unsigned int VAO;

  glGenVertexArrays(1, &VAO);
  
  glBindVertexArray(VAO);

  size_t attr_lens[] = { 3, 3, 2 };

  // data, n vertices, n attr, len of attrs, usage for glBufferData

  unsigned int texture1 = get_texture("assets/container.jpg");
  unsigned int texture2 = get_texture("assets/awesomeface.png");

  glUniform1i(glGetUniformLocation(program, "texture1"), 0);

  glUniform1i(glGetUniformLocation(program, "texture2"), 1);

  unsigned int VBO = get_VBO(vertices, 4, 3, attr_lens, GL_STATIC_DRAW);
  unsigned int EBO = get_EBO(indices, 6, GL_STATIC_DRAW);

  update_time_elapsed();
  
  // render loop

  int quit = 0;

  while (!quit) {
    // update time elapsed (in this case, between frames) and process events
    // (e.g. inputs, window events)

    update_time_elapsed();
    quit = process_events();

    if (was_pressed(27)) {  // 27 is ESC
      quit = 1;
    }

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // primitive to draw, start index, n vertices

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }

  return 0;
}

/*
 *  Set up SDL, return window.
 */

SDL_Window *sdl_setup(const char *window_name) {
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    sdl_perror("SDL_InitSubSystem");
    exit(1);
  }

  atexit(SDL_Quit);

  SDL_GL_LoadLibrary(NULL);

  // OpenGL Version 4.3 (highest supported with my drivers)

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // in gfx_config.h I set up the additional flag(s) (e.g. for fullscreen)

  SDL_Window *window = SDL_CreateWindow(
      window_name,                                       // title 
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,  // x pos, y pos
      (FULLSCREEN ? 0 : INIT_WIDTH),                     // width
      (FULLSCREEN ? 0 : INIT_HEIGHT),                    // height
      SDL_WINDOW_OPENGL | MY_ADDL_SDL_WIN_FLAGS          // flags
    );

  if (!window) {
    sdl_perror("SDL_CreateWindow");
    exit(1);
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (!context) {
    sdl_perror("SDL_GL_CreateContext");
    exit(1);
  }

  // VSYNC

  SDL_GL_SetSwapInterval(1);

  return window;
}

/*
 *  For use with glDebugMessageCallback().
 */

void gl_debug_message(GLenum source, GLenum type, GLuint id, GLenum severity,
                      GLsizei length, const GLchar *message,
                      const void *userParam) {
#if (GL_ONLY_PRINT_ERR)
  if (type == GL_DEBUG_TYPE_ERROR) {
#endif

  fprintf(stderr, "type: 0x%x (%s), severity: 0x%x, message (between ***): "
                  "*** %s ***\n",
          type, (type == GL_DEBUG_TYPE_ERROR ?
                 "error" : "type other than error"), severity, message);

#if (GL_ONLY_PRINT_ERR)
  }
#endif
}

