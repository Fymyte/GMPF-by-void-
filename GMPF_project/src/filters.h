#include "callback.h"

#ifndef FILTERS_H
#define FILTERS_H

//file that contain the prototypes for the GMPF filters functions
void filter_for_selection(void(*filter)(GMPF_Layer *), GtkFlowBox *flowbox);
void filter_for_selection_color(void (*filter)(GMPF_Layer*,
                                                           guchar,
                                                           guchar,
                                                           guchar),
                                                 guchar   r,
                                                 guchar   g,
                                                 guchar   b,
                                                 GtkFlowBox *flowbox);
void GMPF_filter_apply_to_selected_layer(void (*filter)(GMPF_Layer*));
void GMPF_filter_apply_to_all_layer(void (*filter)(GMPF_Layer*));
void GMPF_filter_apply_to_selected_layer_color(void (*filter)(GMPF_Layer*,
                                                                     guchar,
                                                                     guchar,
                                                                     guchar),
                                                           guchar   r,
                                                           guchar   g,
                                                           guchar   b);
void GMPF_filter_apply_to_all_layer_color(void (*filter)(GMPF_Layer*,
                                                                       guchar,
                                                                       guchar,
                                                                       guchar),
                                                             guchar   r,
                                                             guchar   g,
                                                             guchar   b);

void Colorfull(GMPF_Layer *lay);
void Tinter(GMPF_Layer *lay);

void Lightness(GMPF_Layer *lay);
void Darkness(GMPF_Layer *lay);

void Greyscale(GMPF_Layer *lay);
void Negative(GMPF_Layer *lay);
void Binarize(GMPF_Layer *lay);
void BinarizeColor(GMPF_Layer *lay);
void Convolute(GMPF_Layer *lay, double *mat, size_t mat_size);

void Equalize(GMPF_Layer *lay);
void Equalize_color(GMPF_Layer *lay);
void Verticale(GMPF_Layer *lay);
void Horizontale(GMPF_Layer *lay);

void Color_balance(GMPF_Layer *lay, guchar r, guchar g, guchar b);

#endif
