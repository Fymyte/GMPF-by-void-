#include "buffer.h"

// General
/* val ~ [min ; max[ */
#define INC_LOOP(_val, _min, _max) \
        _val++; \
        if (_val == _max) { _val = _min; }

#define DEC_LOOP(_val, _min, _max) \
        if (_val == _min) { _val = _max; } \
        _val--;

// for the buffer
/* val ~ [0 ; BUFFER_SIZE[ */
#define INC_BUF_LOOP(_val) \
        INC_LOOP(_val, 0, BUFFER_SIZE);

#define DEC_BUF_LOOP(_val) \
        DEC_LOOP(_val, 0, BUFFER_SIZE);





#define CLOSE_BUFFER_FILES(_buffer, _begin, _end) \
        if (_begin > _end) \
        { \
            for(; 0 < _end; _end--, _buffer->size--) \
                close(_buffer->elmt[_end]); \
            close(_buffer->elmt[0]); \
            _end = BUFFER_SIZE - 1; \
        } \
        for(; _begin < _end; _end--, _buffer->size--) \
            close(_buffer->elmt[_end]); \
        close(_buffer->elmt[_begin]);



/*
 * Create a new Buffer and attach it to the given flowbox
 * (Return: the newly created Buffer, or NULL if it is unable to malloc)
 */
GMPF_Buffer *GMPF_buffer_init(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = buffer_create();

    if (!buffer)
    {
        PRINTERR ("No created buffer");
        return NULL;
    }

    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, buffer);
    return buffer;
}


/*
 * Destroy the Buffer associated with the given flowbox and free it
 * (Do nothing if there is no Buffer associated)
 */
void GMPF_buffer_destroy(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);

    if (!buffer)
    {
        PRINTERR("Unable to get buffer");
        return;
    }

    buffer_destroy(buffer);
    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, NULL);
}


/*
 * Return the Buffer associated with the given flowbox
 * (Return: the Buffer, or NULL if there is no associated Buffer)
 */
GMPF_Buffer *GMPF_buffer_get_buffer(GtkFlowBox *flowbox)
{
    return (GMPF_Buffer *)g_object_get_data(G_OBJECT(flowbox),
                                                  BUFFER_KEY_NAME);
}


/*
 * Associate the given Buffer to the given flowbox
 * (Return: the previous Buffer, or NULL if they were no one)
 */
GMPF_Buffer *GMPF_buffer_set_buffer(GtkFlowBox  *flowbox,
                                    GMPF_Buffer *buffer)
{
    GMPF_Buffer *prev_buf = GMPF_buffer_get_buffer(flowbox);

    if (!buffer)
    {
        PRINTERR("No created buffer");
        g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, NULL);
        return prev_buf;
    }

    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, buffer);

    return prev_buf;
}


int GMPF_buffer_add(GtkFlowBox *flowbox,
                    GMPF_Action action,
                    GMPF_BufferElement *element)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);
    // return 0;
    return buffer_add(buffer);
}


int GMPF_buffer_undo(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);
    if (!buffer)
    {
        PRINTERR("Unable to get buffer");
        return 1;
    }

    // GMPF_BufferElement *element = buffer_get_current_element(buffer);
    GMPF_Action action = buffer_undo(buffer);
    //
    // switch (action) {
    //     case INCORECT_ACTION: PRINTERR("Enconter an incorect action");
    //                           break;
    //     case MOVE_UP: D_PRINT("MOVE_UP ACTION", NULL);
    //                   gtk_widget_destroy((GtkWidget *) element->layer->UIElement);
    //                   D_PRINT("insert pos: %i", element->pos);
    //                   layer_insert_at_pos(element->layer, flowbox, element->pos);
    //                   break;
    //     case MOVE_DOWN: D_PRINT("MOVE_DOWN ACTION", NULL);
    //                     gtk_widget_destroy((GtkWidget *) element->layer->UIElement);
    //                     D_PRINT("insert pos: %i", element->pos);
    //                     layer_insert_at_pos(element->layer, flowbox, element->pos);
    //                     break;
    //     case MODIF_IMAGE: D_PRINT("MODIF_IMAGE ACTION", NULL);
    //                       break;
    //     case CHANGE_NAME: D_PRINT("CHANGE_NAME ACTION", NULL);
    //                       break;
    //     case DELETE: D_PRINT("DELETE ACTION", NULL);
    //                  break;
    //     case ADD: D_PRINT("ADD ACTION", NULL);
    //               break;
    //     default: D_PRINT("UNKNOWN ACTION", NULL);
    // }

    return 0;
}


int GMPF_buffer_redo(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);
    if (!buffer)
    {
        PRINTERR("Unable to get buffer");
        return 1;
    }

    GMPF_Action action = buffer_redo(buffer);
    // GMPF_BufferElement *element = buffer_get_current_element(buffer);
    //
    // switch (action) {
    //     case INCORECT_ACTION: PRINTERR("Enconter an incorect action");
    //                           break;
    //     case MOVE_UP: D_PRINT("MOVE_UP ACTION", NULL);
    //                gtk_widget_destroy((GtkWidget *) element->layer->UIElement);
    //                D_PRINT("insert pos: %i", element->pos + 1);
    //                layer_insert_at_pos(element->layer, flowbox, element->pos - 1);
    //                break;
    //     case MOVE_DOWN: D_PRINT("MOVE_DOWN ACTION", NULL);
    //                     gtk_widget_destroy((GtkWidget *) element->layer->UIElement);
    //                     D_PRINT("insert pos: %i", element->pos - 1);
    //                     layer_insert_at_pos(element->layer, flowbox, element->pos + 1);
    //                     break;
    //     case MODIF_IMAGE: D_PRINT("MODIF_IMAGE ACTION", NULL);
    //                       break;
    //     case CHANGE_NAME: D_PRINT("CHANGE_NAME ACTION", NULL);
    //                       break;
    //     case DELETE: D_PRINT("DELETE ACTION", NULL);
    //                  break;
    //     case ADD: D_PRINT("ADD ACTION", NULL);
    //               break;
    //     default: D_PRINT("UNKNOWN ACTION", NULL);
    // }
    //
    return 0;
}

/******************************Code for buffer*********************************/

/*
 * Create a new Buffer
 * (Return: the Buffer, or NULL if it is unable to malloc)
 */
GMPF_Buffer *buffer_create()
{
    GMPF_Buffer *buffer = malloc(sizeof(GMPF_Buffer));

    if (!buffer)
    {
        PRINTERR("Unable to malloc");
        return NULL;
    }

    if (buffer_init(buffer))
    { PRINTERR("Unable to init buffer"); }
    return buffer;
}


/*
 * Set all parameters from the given buffer to there default values
 * (Do nothing if the given buffer is invalid)
 * (Return: 0 if there were no error, else 1)
 */
char buffer_init(GMPF_Buffer *buffer)
{
    if (buffer == NULL)
    {
        PRINTERR ("Invalid buffer");
        return 1;
    }
    buffer->begin = 0;
    buffer->end = 0;
    buffer->size = 0;
    buffer->pos = -1;
    for (size_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer->elmt[i] = NULL;
    }
    return 0;
}


/*
 * Close all open filestream in the list and free the given Buffer
 * (Do nothing if the given buffer is invalid)
 */
char buffer_destroy(GMPF_Buffer *buffer)
{
    if (!buffer)
    {
        PRINTERR("Invalid buffer");
        return 1;
    }
    CLOSE_BUFFER_FILES(buffer, buffer->begin, buffer->end);
    free(buffer);
    return 0;
}


/*
 * PURPOSE :  Add a new Action to the Buffer. If a File is provided, then add
 *            it to the file_buffer list, else add a NULL value
 *  PARAMS : GMPF_Buffer *buffer - The buffer in witch add the action
 *           GMPF_Action  action - The action to add
 *           GMPF_BufferElement *elements - the element to add
 * RETURNS : int - 0 if there were no error, else 1
 *   NOTES : Do nothing if the given Buffer or Action is invalid.
 *           Delete the first entered element if the Buffer is full.
 */
char buffer_add(GMPF_Buffer *buffer,
                GMPF_Action action,
                GMPF_Layer *layer)
{
    if (!buffer/* || !action*/ || !layer)
    {
        PRINTERR("Invalid buffer, action or layer");
        return 1;
    }

    if (buffer->size)
    { // size > 0
        if (buffer->pos != buffer->end)
        { // erase the end of the buffer / change buffer->end
            CLOSE_BUFFER_FILES(buffer, INC_BUF_LOOP(buffer->begin), buffer->end);
            buffer->end = buffer->pos;
        }
        else if ((buffer->end + 1) % BUFFER_SIZE == buffer->begin)
        {
            close(buffer->elmt[buffer->begin]);
            INC_BUF_LOOP(buffer->begin);
        }

        // TODO: create the file
        FILE *file;


        INC_BUF_LOOP(buffer->end);
        buffer->elmt[buffer->end] = file;
        INC_BUF_LOOP(buffer->pos);
    }
    else
    { // size == 0
        buffer->begin = 0;
        buffer->end = 0;
        buffer->size = 1;
        buffer->pos = 0;
    }

    D_PRINT("buffer -- pos: %i, begin: %i, end: %i, size: %i",
            buffer->pos, buffer->begin, buffer->end, buffer->size);
    return 0;
}


/*
 * TODO
 */
char buffer_undo(GMPF_Buffer *buffer, GtkFlowBox *flowbox)
{
    if (!buffer->size)
    {
        return 1;
    }
    if (buffer->pos < buffer->begin && buffer->pos >= buffer->end)
    {
        return 1;
    }

    FILE *file = buffer->elmt[buffer->pos];
    // TODO : traiter le fichier des actions
    DEC_LOOP(buffer->pos, 0, BUFFER_SIZE - 1);

    D_PRINT("buffer -- pos: %i, begin: %i, end: %i, size: %i",
            buffer->pos, buffer->begin, buffer->end, buffer->size);

    return 0;
}


/*
 * PURPOSE : Return the current filestream associated with the current action
 *           in the given Buffer
 *  PARAMS : GMPF_Buffer *buffer - The Buffer witch contain the filestream
 * RETURNS : GMPF_BufferElement * - The Element at the current position,
 *           or NULL if there is no assocated Element
 *   NOTES : Do nothing if the buffer is invalid
 */
// GMPF_BufferElement *buffer_get_current_element(GMPF_Buffer *buffer)
// {
//     if (!buffer)
//     { PRINTERR("Invalid Buffer"); return NULL; }
//
//     return buffer->element_buffer[buffer->pos];
// }


/*
 * TODO
 */
char buffer_redo(GMPF_Buffer *buffer, GtkFlowBox *flowbox)
{
    if (!buffer->size)
    {
        return 1;
    }

    if (buffer->pos < buffer->end - 1)
    {
        buffer->pos = (buffer->pos + 1) % BUFFER_SIZE;
        FILE *file = buffer->elmt[buffer->pos];
    }

    D_PRINT("buffer -- pos: %i, begin: %i, end: %i, size: %i",
            buffer->pos, buffer->begin, buffer->end, buffer->size);

    return 0;
}
