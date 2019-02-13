#ifndef HORIZONTAL_H_
#define HORIZONTAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "Struct.h"
#include "sdl_op.h"
#include "Img_rgb_op.h"
#include "Matrix.h"
#include "pixel_operations.h"

void Horizontal(SDL_Surface* image_surface, SDL_Surface* screen_surface);

#endif
