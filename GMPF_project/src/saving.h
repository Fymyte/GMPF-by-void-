#ifndef SAVING_H_
#define SAVING_H_

#include <stdio.h>

#include "GMPF_LayerMngr.h"
#include "GMPF_Struct.h"

#include "macro.h"


void save_project(GMPF_LayerMngr *layermngr, GtkFlowBox *flowbox, const char *filename, char *error);
GMPF_LayerMngr * load_project(GtkFlowBox *flowbox, const char *filename, char *error);

#endif /* SAVING_H_ */
