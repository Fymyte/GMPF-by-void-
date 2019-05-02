#include "buffer.h"


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
GMPF_Buffer *GMPF_buffer_set_buffer(GtkFlowBox        *flowbox,
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
    { PRINTERR ("Unable to init buffer"); }
    return buffer;
}


/*
 * Set all parameters from the given buffer to there default values
 * (Do nothing if the given buffer is invalid)
 * (Return: 0 if there were no error, else 1)
 */
int buffer_init(GMPF_Buffer *buffer)
{
    if (buffer != NULL)
    {
        buffer->begin = 0;
        buffer->size = 0;
        buffer->pos = -1;
        for (size_t i = 0; i < BUFFER_SIZE; i++)
        {
            buffer->buffer[i] = INCORECT_ACTION;
            buffer->element_buffer[i] = NULL;
        }
        return 0;
    }
    else
    {
        PRINTERR ("Invalid buffer");
        return 1;
    }
}


/*
 * Close all open filestream in the list and free the given Buffer
 * (Do nothing if the given buffer is invalid)
 */
void buffer_destroy(GMPF_Buffer *buffer)
{
    if (!buffer)
    {
        PRINTERR("Invalid buffer");
        return;
    }

    free(buffer);
}


/*
 * PURPOSE :  Add a new Action to the Buffer. If a File is provided, then add
 *            it to the file_buffer list, else add a NULL value
 *  PARAMS : GMPF_Buffer *buffer - The buffer in witch add the action
 *           GMPF_Action  action - The action to add
 *           FILE        *file - the filestream to add (Accept NULL value)
 * RETURNS : int - 0 if there were no error, else 1
 *   NOTES : Do nothing if the given Buffer or Action is invalid.
             Delete the first entered element if the Buffer is full.
 */
int buffer_add(GMPF_Buffer *buffer,
                GMPF_Action action,
                GMPF_BufferElement *element)
{
    if (!buffer || action == INCORECT_ACTION)
    {
        PRINTERR("Invalid buffer or action");
        return 1;
    }

    if (buffer->size < BUFFER_SIZE)
        buffer->size += 1;

    buffer->pos = (buffer->pos + 1) % BUFFER_SIZE;

    if (buffer->element_buffer[buffer->pos]->file)
    {
        if (fclose(buffer->element_buffer[buffer->pos]->file))
        { PRINTERR("Unable to close filestream"); }
    }

    if (buffer->size == BUFFER_SIZE)
        buffer->begin = (buffer->begin + 1) % BUFFER_SIZE;
    buffer->buffer[buffer->pos] = action;
    buffer->element_buffer[buffer->pos] = element;

    return 0;
}


/*
 * TODO
 */
GMPF_Action buffer_undo(GMPF_Buffer *buffer)
{
    GMPF_Action action = buffer->buffer[buffer->pos];
    buffer->pos -= 1;
    if (buffer->pos < 0)
        buffer->pos += BUFFER_SIZE;

    return action;
}


/*
 * PURPOSE : Return the current filestream associated with the current action
 *           in the given Buffer
 *  PARAMS : GMPF_Buffer *buffer - The Buffer witch contain the filestream
 * RETURNS : FILE - The filestream at the current position, or NULL if there is
 *                  no assocated filestream
 *   NOTES : Do nothing if the buffer is invalid
 */
GMPF_BufferElement *buffer_get_current_element(GMPF_Buffer *buffer)
{
    if (!buffer)
    { PRINTERR("Invalid Buffer"); return NULL; }

    return buffer->element_buffer[buffer->pos];
}


/*
 * TODO
 */
GMPF_Action buffer_redo(GMPF_Buffer *buffer)
{
    GMPF_Action action = buffer->buffer[buffer->pos];
    buffer->pos = (buffer->pos + 1) % BUFFER_SIZE;
    return action;
}
