# graphics
Learning a bit about graphics with OpenGL and C. I will be using SDL2 to make
things a bit easier in terms of creating a window, etc., but all of the
graphics will be done with OpenGL functions in core profile mode. The final
dependency is GLAD, which is a cross-platform way for me to load the OpenGL
function addresses.\
To build the executable you will need the following: OpenGL 4.3+ capable
drivers, the prerequisites necessary for development with SDL 2.0, GLAD (not
GLAD2) with glad.h at the path glad/glad.h in your includes and glad.c at the
path glad/glad.c inside the repo itself.\
Also added stb\_image.h for loading textures.
