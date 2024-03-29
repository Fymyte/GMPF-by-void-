#ifndef BUFFER_H
#define BUFFER_H

#include "callback.h"
#include "GMPF_Struct.h"
#include "saving.h"

GMPF_Buffer *GMPF_buffer_init(GtkFlowBox *flowbox);
void GMPF_buffer_destroy(GtkFlowBox *flowbox);
void GMPF_buffer_reset(GtkFlowBox *flowbox);
GMPF_Buffer *GMPF_buffer_get_buffer(GtkFlowBox *flowbox);
GMPF_Buffer *GMPF_buffer_set_buffer(GtkFlowBox  *flowbox,
                                    GMPF_Buffer *buffer);
int GMPF_buffer_add(GtkFlowBox *flowbox,
                    GMPF_Action action,
                    GMPF_Layer *layer);
int GMPF_buffer_undo(GtkFlowBox *flowbox);
int GMPF_buffer_redo(GtkFlowBox *flowbox);



//
// BUFFER FUNCTIONS
//
GMPF_Buffer *buffer_create();
char buffer_init(GMPF_Buffer *buffer);
char buffer_destroy(GMPF_Buffer *buffer);

char buffer_add(GMPF_Buffer *buffer,
                GMPF_Action action,
                GMPF_Layer *layer);
//GMPF_BufferElement *buffer_get_current_element(GMPF_Buffer *buffer);
char buffer_undo(GMPF_Buffer *buffer, GtkFlowBox *flowbox);
char buffer_redo(GMPF_Buffer *buffer, GtkFlowBox *flowbox);

#endif /* BUFFER_H */
