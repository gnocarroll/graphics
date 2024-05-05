#include "event_handling.h"

#include <string.h>

#include <glad/glad.h>
#include <SDL.h>

static int modify_keyboard_state(SDL_KeyboardEvent);

static int enqueue_char(SDL_Keysym);
static int dequeue_char(void);

enum input_info_type {
  KEYBOARD_INPUT_INFO,
  MOUSE_INPUT_INFO
};

typedef struct mouse_info {
  // SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
  // SDL_BUTTON_X1, SDL_BUTTON_X2;

  Uint8 button;
  Uint8 clicks;  // n clicks corresponding to certain action
} mouse_info;

/*
 *  struct input_info describes the input corresponding to a certain action,
 *  e.g. moving the player forward
 */

typedef struct input_info {
  Uint8 type;

  union {
    SDL_Keysym keysym;  // For keyboard input
    mouse_info;  // For mouse input
  };
} input_info;

input_info in_mappings[N_INPUT_TYPES] = {
  [QUIT] = { KEYBOARD_INPUT_INFO, .keysym.sym = '\x1B' }  // ESC key for now
};

// for recording states of keys/inputs
// counter for each thing program should pay attention to

typedef Uint8 input_state_arr[N_INPUT_TYPES];

// key_states:   is key currently pressed? Persists across frames.
// key_pressed:  n times pressed this frame
// key_released: n times released this frame

static input_state_arr input_states;
static input_state_arr input_pressed;
static input_state_arr input_released;

// queue for ASCII characters, may be useful if typing necessary in game

#define ASCII_QUEUE_LEN (256)

static unsigned char ascii_queue[ASCII_QUEUE_LEN];
static unsigned front = 0;
static unsigned back = 0;

/*
 *  Processes all events. Return value indicates if program should quit.
 */

int process_events(void) {
  memset(&input_pressed, 0, sizeof(input_pressed));
  memset(&input_released, 0, sizeof(input_released));

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
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        // TODO: modify input state for mouse button press
        break;
    }
  }

  return 0;
}

/*
 *  Modify structs that record state of input using SDL_KeyboardEvent.
 */

static int modify_keyboard_state(SDL_KeyboardEvent key) {
  Uint8 state_bool = (Uint8) (key.type == SDL_KEYDOWN);
  input_state_arr *state_arr = (key.type == SDL_KEYDOWN ?
                                &input_pressed : &input_released);

  // queue is maintained for the purposes of maybe using some form of typing
  // in game (e.g. messaging, configuring something)
  // separate from other input processing

  if (state_bool) {
    enqueue_char(key.keysym);
  }

  for (int i = 0; i < N_INPUT_TYPES; i++) {
    if ((in_mappings[i].type == KEYBOARD_INPUT_INFO) &&
        (key.keysym.sym == in_mappings[i].keysym.sym)) {
      input_states[i] = state_bool;
      (*state_arr)[i]++;
    }
  }

  return 0;
}

/*
 *  Functions below are for working with ascii_queue.
 */

#define NEXT_QUEUE_IDX(i) ((i + 1) % ASCII_QUEUE_LEN)

static int enqueue_char(SDL_Keysym keysym) {
  if ((keysym.sym <= 0) || (keysym.sym > 127)) {
    return -1;  // not ascii
  }

  unsigned char c = (unsigned char) keysym.sym;

  if (NEXT_QUEUE_IDX(back) == front) {
    // keep back from becoming front (would imply empty queue)
    // effectively we are discarding from front of queue here

    front = NEXT_QUEUE_IDX(front);
  }

  ascii_queue[back] = c;  // add to back
  back = NEXT_QUEUE_IDX(back);

  return 0;
}

/*
 *  -1  => no char
 *  > 0 => unsigned char cast to int
 */

static int dequeue_char(void) {
  if (front == back) {
    return -1;
  }

  int ret = (int) ((unsigned char) ascii_queue[front]);

  front = NEXT_QUEUE_IDX(front);

  return ret;
}

void clear_ascii_queue(void) {
  front = 0;
  back = 0;
}

/*
 *  Non-static wrapper for above so program can pull characters out of queue.
 */

int getc_ascii(void) {
  return dequeue_char();
}

int is_pressed_now(int input_type) {
  if ((input_type < 0) || (input_type >= N_INPUT_TYPES)) {
    return 0;
  }

  return input_states[input_type]; 
}

int was_pressed(int input_type) {
  if ((input_type < 0) || (input_type >= N_INPUT_TYPES)) {
    return 0;
  }

  return input_pressed[input_type]; 
}

int was_released(int input_type) {
  if ((input_type < 0) || (input_type >= N_INPUT_TYPES)) {
    return 0;
  }

  return input_released[input_type]; 
}

