#ifndef EVENT_HANDLING_H
#define EVENT_HANDLING_H

/*
 *  Process all SDL events. Return value indicates if program should quit.
 */

int process_events(void);

/*
 *  Key inputs are also saved in ascii array, e.g. for implementing typing for
 *  some kind of messaging or configuring.
 *  When getting text input should probably pull everything out every frame or
 *  at least frequently since queue will just dump chars if it gets full.
 */

#define ASCII_QUEUE_LEN (256)

void start_text_input(void);
void stop_text_input(void);

void clear_text_input(void);  // empty queue
int getc_text_input(void);    // get character from front of queue

// only part of ascii space is being used for the purposes of my text input
// so I will define these constants for left/right arrow key information

#define L_ARROW_CHAR ((unsigned char) 1)
#define R_ARROW_CHAR ((unsigned char) 2)

/*
 *  Seeing what is/was pressed. Right now I don't think "standard" key bindings
 *  will have the ability to use a combination of keys e.g. CTRL + A. To use
 *  these three functions below, either provide the ASCII of a character like
 *  doing was_pressed('a') or use one of the input_enum constants for those
 *  additional inputs. Also, use lowercase letters and in general the key
 *  without applying SHIFT to use these functions.
 */

int is_pressed_now(int);
int was_pressed(int);
int was_released(int);

/*
 *  Mouse click information
 */

int get_first_mouse_click_x(void);
int get_first_mouse_click_y(void);
int get_last_mouse_click_x(void);
int get_last_mouse_click_y(void);

/*
 *  Mouse position information
 */

int get_mouse_x(void);
int get_mouse_y(void);

/*
 *  enum for querying and storing what inputs occurred
 */

typedef enum input_enum {
  // first 127 will be ascii, can query with corresponding char

  L_ARROW = 128,
  R_ARROW,
  U_ARROW,
  D_ARROW,
  INPUT_F1,
  INPUT_F2,
  INPUT_F3,
  INPUT_F4,
  INPUT_F5,
  INPUT_F6,
  INPUT_F7,
  INPUT_F8,
  INPUT_F9,
  INPUT_F10,
  INPUT_F11,
  INPUT_F12,
  INPUT_F13,
  INPUT_F14,
  INPUT_F15,
  CAPSLOCK,
  RSHIFT,
  LSHIFT,
  RCTRL,
  LCTRL,
  RALT,
  LALT,

  // mouse

  MOUSE1,
  MOUSE2,
  MOUSE3,
  MOUSE4,
  MOUSE5,

  N_INPUT_TYPES
} input_enum;


#endif // EVENT_HANDLING_H
