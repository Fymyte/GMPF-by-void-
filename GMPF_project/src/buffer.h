#ifndef BUFFER_H
#define BUFFER_H

# include "callback.h"
# include "GMPF_Struct.h"

GMPF_Buffer *GMPF_buffer_init(GtkFlowBox *flowbox);
void GMPF_buffer_destroy(GtkFlowBox *flowbox);
GMPF_Buffer *GMPF_buffer_get_buffer(GtkFlowBox *flowbox);
GMPF_Buffer *GMPF_buffer_set_buffer(GtkFlowBox  *flowbox,
                                    GMPF_Buffer *buffer);
int GMPF_buffer_add(GtkFlowBox *flowbox,
                    GMPF_Action action,
                    GMPF_BufferElement *element);
int GMPF_buffer_undo(GtkFlowBox *flowbox);
int GMPF_buffer_redo(GtkFlowBox *flowbox);

GMPF_Buffer *buffer_create();
int buffer_init(GMPF_Buffer *buffer);
void buffer_destroy(GMPF_Buffer *buffer);

int buffer_add(GMPF_Buffer *buffer/*,
               GMPF_Action  action,
               GMPF_BufferElement *element*/);
GMPF_BufferElement *buffer_get_current_element(GMPF_Buffer *buffer);
GMPF_Action buffer_undo(GMPF_Buffer *buffer);
GMPF_Action buffer_redo(GMPF_Buffer *buffer);

#endif /* BUFFER_H */
