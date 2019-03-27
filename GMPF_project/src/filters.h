#include "callback.h"

#ifndef FILTERS_H
#define FILTERS_H

//file that contain the prototypes for the GMPF filters functions
void Greyscale(SGlobalData *data);
void Negative(SGlobalData *data);
void Binarize(SGlobalData *data);
void BinarizeColor(SGlobalData *data);
void Tinter(SGlobalData *data);

#endif
