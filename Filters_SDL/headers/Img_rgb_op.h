#ifndef IMG_RGB_OP_H_
#define IMG_RGB_OP_H_

#include <stdio.h>
#include <stdlib.h>

#include "Struct.h"
#include "Matrix.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "pixel_operations.h"

struct Img_rgb *init_img_rgb(int rows, int cols);

void img_rgb_zero(struct Img_rgb *img);

void free_img_rgb(struct Img_rgb *img);

void Img_rgb_to_Image(SDL_Surface* image_surface, struct Img_rgb *img);

#endif
