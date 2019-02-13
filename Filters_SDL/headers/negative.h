#ifndef NEGATIVE_H_
#define NEGATIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "pixel_operations.h"
#include "sdl_op.h"

void negative(SDL_Surface *image_surface, SDL_Surface* screen_surface);

#endif

