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
        return;

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
        return NULL;

    buffer_init(buffer);
    return buffer;
}


/*
 * Set all parameters from the given buffer to there default values
 * (Do nothing if the given buffer is invalid)
 */
void buffer_init(GMPF_Buffer *buffer)
{
    if (buffer != NULL)
    {
        buffer->begin = 0;
        buffer->size = 0;
        buffer->pos = -1;
    }
}


/*
 * Close all open filestream in the list and free the given Buffer
 */
void buffer_destroy(GMPF_Buffer *buffer)
{
    if (!buffer)
        return;

    free(buffer);
}


/*
 * Add a new Action to the Buffer, if a File is provided, then add it to the
 * file_buffer list, else add a NULL value
 * (Do nothing if the given Buffer or Action is invalid)
 * (Delete the first entered element if the Buffer is full)
 * (Return: 0 if there were no error, else 1)
 */
int buffer_add(GMPF_Buffer *buffer,
                GMPF_Action  action,
                FILE        *file)
{
    if (!buffer || action == INCORECT_ACTION)
        return 0;

    buffer->pos = (buffer->pos + 1) % BUFFER_SIZE;

    if (buffer->file_buffer[buffer->pos])
    {
        int err = fclose(buffer->file_buffer[buffer->pos]);
        if (err)
        {
            PRINTERR("Unable to close filestream");
            return 1;
        }
    }

    buffer->begin = (buffer->begin + 1) % BUFFER_SIZE;
    buffer->buffer[buffer->pos] = action;
    buffer->file_buffer[buffer->pos] = file;

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


FILE *GMPF_buffer_get_current_file(GMPF_Buffer *buffer)
{
    return buffer->file_buffer[buffer->pos];
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
