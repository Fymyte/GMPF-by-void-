#ifndef SAVING_H_
#define SAVING_H_

#include <stdio.h>

#include "GMPF_LayerMngr.h"
#include "GMPF_Struct.h"

#include "macro.h"


char save_project(GtkFlowBox *flowbox, const char *filename);
char load_project(GtkFlowBox *flowbox, const char *filename);

char saving_layer(GtkFlowBox *flowbox, const char *filename);
char loading_layer(GtkFlowBox *flowbox, const char *filename);

#endif /* SAVING_H_ */
