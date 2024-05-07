#ifndef EVENT_HANDLING_H
#define EVENT_HANDLING_H

/*
 *  Process all SDL events. Return value indicates if program should quit.
 */

int process_events(void);

/*
 *  Key inputs are also saved in ascii array, e.g. for implementing typing for
 *  some kind of messaging or configuring.
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
 *  Seeing what is/was pressed.
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
 *  enum for actions that player's input can correspond to
 */

typedef enum input_enum {
  // Configurable inputs

  QUIT = 0,
  LEFT,
  RIGHT,
  FORWARD,
  BACK,
  ATTACK1,
  ATTACK2,

  // Non-configurable inputs (e.g. for navigating menus)

  L_ARROW,
  R_ARROW,
  U_ARROW,
  D_ARROW,

  MOUSE1,
  MOUSE2,

  // Provides number of values enum has

  N_INPUT_TYPES
} input_enum;

#endif // EVENT_HANDLING_H
