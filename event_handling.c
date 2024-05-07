#include "event_handling.h"

#include <string.h>

#include <glad/glad.h>
#include <SDL.h>

// for each event processed, there is switch statement on type
// corresponding one of these will be called to handle event

static int key_handler(SDL_KeyboardEvent);
static int mouse_button_handler(SDL_MouseButtonEvent);
static int mouse_motion_handler(SDL_MouseMotionEvent);
static int mouse_wheel_handler(SDL_MouseWheelEvent);
static int text_input_handler(SDL_TextInputEvent);

// add/remove from text input queue

static int enqueue_char_keysym(SDL_Keysym);
static int enqueue_char(unsigned char);
static int dequeue_char(void);

enum input_info_type {
  KEYBOARD_INPUT_INFO,
  MOUSE_PRESS_INFO
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
    mouse_info m_info;  // For mouse input
  };
} input_info;

input_info in_mappings[N_INPUT_TYPES] = {
  [QUIT]    = { KEYBOARD_INPUT_INFO, .keysym.sym = '\x1B' },  //

  [L_ARROW] = { KEYBOARD_INPUT_INFO, .keysym.sym = SDLK_LEFT },
  [R_ARROW] = { KEYBOARD_INPUT_INFO, .keysym.sym = SDLK_RIGHT },
  [U_ARROW] = { KEYBOARD_INPUT_INFO, .keysym.sym = SDLK_UP },
  [D_ARROW] = { KEYBOARD_INPUT_INFO, .keysym.sym = SDLK_DOWN },

  [MOUSE1]  = { MOUSE_PRESS_INFO, .m_info.button = SDL_BUTTON_LEFT },
  [MOUSE2]  = { MOUSE_PRESS_INFO, .m_info.button = SDL_BUTTON_RIGHT }
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

// record mouse x, y

static Sint32 first_mouse_click_x;
static Sint32 first_mouse_click_y;

static Sint32 last_mouse_click_x;
static Sint32 last_mouse_click_y;

// current mouse coords

static Sint32 mouse_x;
static Sint32 mouse_y;

// net (or total) relative mouse movement during frame

static Sint32 net_xrel;
static Sint32 net_yrel;

// mouse wheel

static Sint32 net_wheel_x;
static Sint32 net_wheel_y;

// queue for ASCII characters, may be useful if typing necessary in game

static unsigned char ascii_queue[ASCII_QUEUE_LEN];
static unsigned front = 0;
static unsigned back = 0;

static int is_on_ascii_queue = 0;

/*
 *  Processes all events. Return value indicates if program should quit.
 */

int process_events(void) {
  memset(&input_pressed, 0, sizeof(input_pressed));
  memset(&input_released, 0, sizeof(input_released));

  first_mouse_click_x = -1;

  net_xrel = 0;
  net_yrel = 0;
  net_wheel_x = 0;
  net_wheel_y = 0;

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
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
        key_handler(event.key);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        mouse_button_handler(event.button);
        break;
      case SDL_MOUSEMOTION:
        mouse_motion_handler(event.motion);
        break;
      case SDL_MOUSEWHEEL:
        mouse_wheel_handler(event.wheel);
        break;
      case SDL_TEXTINPUT:
        text_input_handler(event.text);
        break;
      default:
        break;
    }
  }

  return 0;
}

/*
 *  Modify structs that record state of input using SDL_KeyboardEvent.
 */

static int key_handler(SDL_KeyboardEvent key) {
  Uint8 state_bool = (Uint8) (key.type == SDL_KEYDOWN);
  input_state_arr *state_arr = (state_bool ?
                                &input_pressed : &input_released);

  // queue is maintained for the purposes of maybe using some form of typing
  // in game (e.g. messaging, configuring something)
  // separate from other input processing

  if ((is_on_ascii_queue) && (state_bool)) {
    // SDL_TEXTINPUT will handle most inputs from user, but here we will get
    // things that are not captured there (see below)

    // get as int to compare against int constants

    int keycode = (int) key.keysym.sym;

    if ((keycode == 8) || (keycode == 13) ||    // backspace, enter
        (keycode == 27) || (keycode == 127)) {  // ESC, delete
      enqueue_char((unsigned char) keycode);
    }
    else if (key.keysym.sym == SDLK_LEFT) {     // left arrow
      enqueue_char(L_ARROW_CHAR);
    }
    else if (key.keysym.sym == SDLK_RIGHT) {    // right arrow
      enqueue_char(R_ARROW_CHAR);
    }
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

static int mouse_button_handler(SDL_MouseButtonEvent button) {
  Uint8 state_bool = (Uint8) (button.type == SDL_MOUSEBUTTONDOWN);
  input_state_arr *state_arr = (state_bool ?
                                &input_pressed : &input_released);
  
  for (int i = 0; i < N_INPUT_TYPES; i++) {
    if ((in_mappings[i].type == MOUSE_PRESS_INFO) &&
        (button.button == in_mappings[i].m_info.button)) {
      input_states[i] = state_bool;
      (*state_arr)[i]++;
    }
  }

  if (state_bool) {
    if (first_mouse_click_x == -1) {  // Has not been modified yet for frame
      first_mouse_click_x = button.x;
      first_mouse_click_y = button.y;
    }

    last_mouse_click_x = button.x;
    last_mouse_click_y = button.y;
  }

  return 0;
}

static int mouse_motion_handler(SDL_MouseMotionEvent motion) {
  mouse_x = motion.x;
  mouse_y = motion.y;

  net_xrel += motion.xrel;
  net_yrel += motion.yrel;

  return 0;
}

static int mouse_wheel_handler(SDL_MouseWheelEvent wheel) {
  if (wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
    net_wheel_x -= wheel.x;
    net_wheel_y -= wheel.y;
  }
  else {
    net_wheel_x += wheel.x;
    net_wheel_y += wheel.y;
  }

  return 0;
}

/*
 *  Add all characters in event to input queue. Most characters except for a
 *  few like backspace and delete will come through here. SDL takes care of
 *  stuff like applying shift or caps lock properly to the characters.
 */

static int text_input_handler(SDL_TextInputEvent text) {
  for (size_t i = 0; (i < SDL_TEXTINPUTEVENT_TEXT_SIZE) && (text.text[i]);
       i++) {
    enqueue_char((unsigned char) text.text[i]);
  }
}

/*
 *  Functions below are for working with ascii_queue.
 */

#define NEXT_QUEUE_IDX(i) ((i + 1) % ASCII_QUEUE_LEN)

void start_text_input(void) {
  SDL_StartTextInput();
  is_on_ascii_queue = 1;
}

void stop_text_input(void) {
  SDL_StopTextInput();
  is_on_ascii_queue = 0;
}

static int enqueue_char_keysym(SDL_Keysym keysym) {
  if ((keysym.sym <= 0) || (keysym.sym > 127)) {
    return -1;  // not ascii
  }

  return enqueue_char((unsigned char) keysym.sym);
}

static int enqueue_char(unsigned char c) {
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

void clear_text_input(void) {
  front = 0;
  back = 0;
}

/*
 *  Non-static wrapper for dequeue_char() above so program can pull characters
 *  out of queue.
 */

int getc_text_input(void) {
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

int get_first_mouse_click_x(void) {
  return first_mouse_click_x;
}

int get_first_mouse_click_y(void) {
  return first_mouse_click_y;
}

int get_last_mouse_click_x(void) {
  return last_mouse_click_x;
}

int get_last_mouse_click_y(void) {
  return last_mouse_click_y;
}

