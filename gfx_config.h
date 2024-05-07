#ifndef GFX_CONFIG_H
#define GFX_CONFIG_H

// Window options

#define FULLSCREEN  (0)
#define RESIZABLE   (1)

// If not fullscreen, initial width/height

#define INIT_WIDTH  (640)
#define INIT_HEIGHT (480)

// Set up additonal flags for creating window with SDL based on options

#if (FULLSCREEN)
  #define FS_FLAG SDL_WINDOW_FULLSCREEN_DESKTOP
#else
  #define FS_FLAG (0)
#endif

#if (RESIZABLE)
  #define RS_FLAG SDL_WINDOW_RESIZABLE
#else
  #define RS_FLAG (0)
#endif

// Flag variable for use while creating window

#define MY_ADDL_SDL_WIN_FLAGS (FS_FLAG | RS_FLAG)

// debug options

#define SDL_DEBUG (1)
#define GL_DEBUG  (1)

#define GL_ONLY_PRINT_ERR (1)

#endif // GFX_CONFIG_H
