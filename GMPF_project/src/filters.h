#include "callback.h"

#ifndef FILTERS_H
#define FILTERS_H

//file that contain the prototypes for the GMPF filters functions
void filter_for_selection_unop(void(*filter)(GMPF_Layer *), GtkFlowBox *flowbox);
void filter_for_selection(void(*filter)(GMPF_Pixel *), GtkFlowBox *flowbox);
void filter_for_selection_color(void (*filter)(GMPF_Layer*,
                                                           guchar,
                                                           guchar,
                                                           guchar),
                                                 guchar   r,
                                                 guchar   g,
                                                 guchar   b,
                                                 GtkFlowBox *flowbox);
void GMPF_filter_apply_to_selected_layer_unop(void (*filter)(GMPF_Layer*));
void GMPF_filter_apply_to_selected_layer(void (*filter)(GMPF_Pixel*));
void GMPF_filter_apply_to_all_layer_unop(void (*filter)(GMPF_Layer*));
void GMPF_filter_apply_to_all_layer(void (*filter)(GMPF_Pixel*));
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

void Colorfull(GMPF_Pixel *p);
void Tinter(GMPF_Pixel *p);

void Lightness(GMPF_Pixel *p);
void Darkness(GMPF_Pixel *p);

void Greyscale(GMPF_Pixel *p);
void Negative(GMPF_Pixel *p);
void Binarize(GMPF_Pixel *p);
void BinarizeColor(GMPF_Pixel *p);
void Convolute(GMPF_Layer *lay, double *mat, size_t mat_size);

void Equalize(GMPF_Layer *lay);
void Equalize_color(GMPF_Layer *lay);
void Verticale(GMPF_Layer *lay);
void Horizontale(GMPF_Layer *lay);

void Color_balance(GMPF_Layer *lay, guchar r, guchar g, guchar b);

void pixelFilter(GMPF_Layer *lay, GMPF_Size *size, void (*filter)(GMPF_Pixel*));

#endif
