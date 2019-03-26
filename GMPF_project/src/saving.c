#include "saving.h"

//
// private functions declaration
//


//
// MAIN FUNCTIONS
//
/*void save_project(GMPF_LayerMngr *layermngr, GtkFlowBox *flowbox, const char *filename, char *error)
{
    // write the file / delete it if it already exists
    FILE *file = fopen(filename, "wb"); // write as binary => rb



    fclose(file);
}

GMPF_LayerMngr * load_project(GtkFlowBox *flowbox, const char *filename, char *error)
{
    // only read the file
    FILE *file = fopen(filename, "rb"); // read as binary => rb
    if (file == NULL) { *error == 1; }

    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    SAFE_MALLOC(GMPF_LayerMngr, layermngr, error);


    layermngr_initialization(layermngr);

    layermngr_set_to_flowbox(flowbox, layermngr); // link flowbox and layermngr


    // READ THE FILE


    fclose(file);
}*/

//
// PRIVATE FUNCTIONS
//

