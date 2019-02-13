#ifndef BINARIZE_H_
#define BINARIZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "pixel_operations.h"
#include "sdl_op.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void binarize(SDL_Surface* image_surface, SDL_Surface *screen_surface);

#endif
