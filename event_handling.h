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

void clear_ascii_queue(void);
int getc_ascii(void);

/*
 *  Seeing what is/was pressed.
 */

int is_pressed_now(int);
int was_pressed(int);
int was_released(int);

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
  MOUSE3,
  MOUSE4,

  // Provides number of values enum has

  N_INPUT_TYPES
} input_enum;

#endif // EVENT_HANDLING_H
