#include "GMPF_Struct.h"



// GMPF_Layer
GMPF_Layer *init_GMPF_Layer(GMPF_LayerMngr *layermngr)
{
}

void free_GMPF_Layer(GMPF_Layer *layer, GMPF_LayerMngr *layermngr)
{
}


// GMPF_LayerMngr
GMPF_LayerMngr *init_GMPF_LayerMngr()
{
    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    layermngr->nb_layer = 0;
    layermngr->layer_list = NULL;
    
}

void free_GMPF_LayerMngr(GMPF_LayerMngr *layermngr)
{
}


