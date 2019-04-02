#include "callback.h"

#ifndef FILTERS_H
#define FILTERS_H

//file that contain the prototypes for the GMPF filters functions

void Lightness(SGlobalData *data);
void Darkness(SGlobalData *data);

void Greyscale(SGlobalData *data);
void Negative(SGlobalData *data);
void Binarize(SGlobalData *data);
void BinarizeColor(SGlobalData *data);
void Tinter(SGlobalData *data);
void Colorfull(SGlobalData *data);
void Convolute(SGlobalData *data,double *mat);

void Equalize(SGlobalData *data);
void Equalize_color(SGlobalData *data);
void Verticale(SGlobalData *data);
void Horizontale(SGlobalData *data);

#endif
