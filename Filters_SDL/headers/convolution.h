#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "Struct.h"
#include "sdl_op.h"
#include "Img_rgb_op.h"
#include "Matrix.h"

void Convolution(SDL_Surface *image_surface, SDL_Surface *screen_surface);

#endif
