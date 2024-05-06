#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <SDL.h>

#include "gfx_config.h"
#include "gfx_util.h"
#include "event_handling.h"
#include "shaders.h"

SDL_Window *sdl_setup(const char *);

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

  int w;
  int h;

  SDL_GetWindowSize(window, &w, &h);

  glViewport(0, 0, w, h);

  // relative mode hides cursor and constrains mouse position to window
  // would be used in FPS or something like that

  // SDL_SetRelativeMouseMode(SDL_TRUE);

  int quit = 0;

  float vertices[] = {
    -0.5, -0.5, 0.0,
     0.5, -0.5, 0.0,
     0.0,  0.5, 0.0
  };

  unsigned int vertexShader = get_vertex_shader("vertex.glsl");
  unsigned int fragShader = get_frag_shader("frag.glsl");

  if ((!vertexShader) || (!fragShader)) {
    fprintf(stderr, "missing shader\n");
    exit(1);
  }

  unsigned int arr[2] = { vertexShader, fragShader };
  unsigned int program = get_program(arr, 2);

  delete_shaders(arr, 2);
  
  if (!program) {
    fprintf(stderr, "creating shader program failed\n");
    exit(1);
  }

  glUseProgram(program);
  
  unsigned int VAO;

  glGenVertexArrays(1, &VAO);
  
  glBindVertexArray(VAO);

  unsigned int VBO;

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *) 0);
  glEnableVertexAttribArray(0);

  while (!quit) {
    quit = process_events();

    if (was_pressed(QUIT)) {
      quit = 1;
    }

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // primitive to draw, start index, n vertices

    glDrawArrays(GL_TRIANGLES, 0, 3);

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

