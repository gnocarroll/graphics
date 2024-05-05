#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <SDL.h>

#include "gfx_util.h"

#define FULLSCREEN  (0)

#define INIT_WIDTH  (640)
#define INIT_HEIGHT (480)

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

  int quit = 0;

  while (!quit) {
    // Maybe at some point will setup more advanced event handling, for now
    // just used to see if loop should stop

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

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

  SDL_Window *window;

  // SDL_CreateWindow(const char *title, int x, int y, int w, int h,
  //                  Uint32 flags);

  if (FULLSCREEN) {
    window = SDL_CreateWindow(window_name, 
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              0, 0,
                              SDL_WINDOW_FULLSCREEN_DESKTOP |
                              SDL_WINDOW_OPENGL);
  }
  else {
    window = SDL_CreateWindow(window_name, 
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              INIT_WIDTH, INIT_HEIGHT, SDL_WINDOW_OPENGL);
  }

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

