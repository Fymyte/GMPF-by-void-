#ifndef SAVING_H_
#define SAVING_H_

#include <stdio.h>

#include "GMPF_LayerMngr.h"
#include "GMPF_Struct.h"

#include "callback.h"

char *get_extension(char *filename);
int check_extension(char **filename, char *extension);
int set_extension(char **filename, char *extension);

char save_project(GtkFlowBox *flowbox, const char *filename);
char load_project(GtkFlowBox *flowbox, const char *filename);

char saving_layer(GtkFlowBox *flowbox, const char *filename);
char loading_layer(GtkFlowBox *flowbox, const char *filename);

int export_cairo_to_png(gchar *filename, gpointer user_data);

#endif /* SAVING_H_ */
