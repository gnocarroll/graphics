# Makefile for graphics repo

GOAL = learn_gfx

# Using SDL2 for windowing, so need these

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDLIBS := $(shell sdl2-config --libs)

CC = gcc
CFLAGS := -O3 -g $(SDL_CFLAGS)
LDLIBS := -lm -lGL $(SDL_LDLIBS)

all: $(GOAL)

# Generate object files

objects=main.o glad.o event_handling.o shaders.o gfx_util.o linalg.o textures.o stb_image.o
objects_w_hdr=

$(objects): %.o: %.c
$(objects_w_hdr): %.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

# Generate executable

$(GOAL): $(objects) $(objects_w_hdr)
	$(CC) $^ $(LDLIBS) -o $@

# Other

linalg_test: linalg.h linalg.c linalg_test.c
	$(CC) -g linalg.c linalg_test.c -o $@

executables = $(GOAL) linalg_test

.PHONY: clean
clean:
	rm -f $(executables) *.o

