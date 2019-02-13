#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "pixel_operations.h"
#include "sdl_op.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void grayscale(SDL_Surface* image_surface, SDL_Surface *screen_surface);

#endif
