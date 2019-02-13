#ifndef BINARIZE_COLOR_H_
#define BINARIZE_COLOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "sdl_op.h"
#include "pixel_operations.h"

void binarize_color(SDL_Surface* image_surface, SDL_Surface *screen_surface);

#endif
