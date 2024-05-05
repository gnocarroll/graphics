#include "event_handling.h"

#include <string.h>

#include <glad/glad.h>
#include <SDL.h>

static int modify_keyboard_state(SDL_KeyboardEvent);

// struct for recording states of keys/inputs
// counter for each thing program should pay attention to

typedef struct key_state_s {
  Uint8 esc;
  Uint8 space;
  Uint8 tab;
  Uint8 ret;
  Uint8 backspace;

  Uint8 ctrl;
  Uint8 shift;
  Uint8 alt;

  Uint8 printable[96];

  Uint8 up;
  Uint8 down;
  Uint8 left;
  Uint8 right;

  Uint8 fn[15];

  Uint8 caps_lock;

  // Modifiers

  Uint8 kmod_caps; // specifically caps lock (does not refer to shift)

  Uint8 kmod_ctrl;
  Uint8 kmod_shift;
  Uint8 kmod_alt;

  Uint8 kmod_lctrl;
  Uint8 kmod_rctrl;
  Uint8 kmod_lshift;
  Uint8 kmod_rshift;
  Uint8 kmod_lalt;
  Uint8 kmod_ralt;
} key_state_s;

// key_states:   is key currently pressed? Persists across frames.
// key_pressed:  n times pressed this frame
// key_released: n times released this frame

static key_state_s key_states;
static key_state_s key_pressed;
static key_state_s key_released;

/*
 *  Processes all events. Return value indicates if program should quit.
 */

int process_events(void) {
  // Reset key_pressed, key_released every time inputs are processed

  memset(&key_pressed, 0, sizeof(key_pressed));
  memset(&key_released, 0, sizeof(key_released));

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        return 1;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          return 1;
        }
        else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          glViewport(0, 0, event.window.data1, event.window.data2);
        }
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        modify_keyboard_state(event.key);
        break;
    }
  }

  return 0;
}

static int modify_keyboard_state(SDL_KeyboardEvent key) {
  Uint8 state_bool = (Uint8) (key.type == SDL_KEYDOWN);
  key_state_s *state_s = (key.type == SDL_KEYDOWN ?
                          &key_pressed : &key_released);

  if (key.type == SDL_KEYDOWN) {
    printf("Key information\n");

    if (key.keysym.sym == SDLK_a) {
      printf("Is a\n");
    }
  }

  return 0;
}

