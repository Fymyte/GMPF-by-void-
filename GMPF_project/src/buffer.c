#include "buffer.h"


/*
 * Create a new Buffer and attach it to the given flowbox
 * (Return: the newly created Buffer, or NULL if it is unable to malloc)
 */
s_buffer *GMPF_buffer_init(GtkFlowBox *flowbox)
{
    s_buffer *buff = buffer_create();
    if (!buff)
    {
        PRINTERR;
        return;
    }
    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, buff);
    return buff;
}


/*
 * Destroy the Buffer associated with the given flowbox and free it
 * (Do nothing if there is no Buffer associated)
 */
void GMPF_buffer_destroy(GtkFlowBox *flowbox)
{
    s_buffer *buff = GMPF_buffer_get_buffer(flowbox);
    if (!buff)
        return;
    buffer_destroy(buff);
    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, NULL);
}


/*
 * Return the Buffer associated with the given flowbox
 * (Return: the Buffer, or NULL if there is no associated Buffer)
 */
s_buffer *GMPF_buffer_get_buffer(GtkFlowBox *flowbox)
{
    return (s_buffer *)g_object_get_data(G_OBJECT(flowbox), BUFFER_KEY_NAME);
}


/*
 * Associate the given Buffer to the given flowbox
 * (Return: the previous Buffer, or NULL if they were no one)
 */
s_buffer *GMPF_buffer_set_buffer(GtkFlowBox *flowbox, s_buffer *buff)
{
    s_buffer *prev_buf = GMPF_buffer_get_buffer(flowbox);
    if (!buff)
    {
        PRINTERR;
        g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, NULL);
        return prev_buf;
    }
    g_object_set_data(G_OBJECT(flowbox), BUFFER_KEY_NAME, buff);
    return prev_buf;
}

/******************************Code for buffer*********************************/

/*
 * Create a new Buffer
 * (Return: the Buffer, or NULL if it is unable to malloc)
 */
s_buffer *buffer_create()
{
    s_buffer *buf = malloc(sizeof(s_buffer));
    if (!buf)
        return NULL;
    buffer_init(buf);
    return buf;
}


/*
 * Set all parameters from the given buffer to there default values
 * (Do nothing if the given buffer is invalid)
 */
void buffer_init(s_buffer *buf)
{
    if (buf != NULL)
    {
        buf->begin = 0;
        buf->size = 0;
        buf->pos = -1;
    }
}


/*
 * Close all open filestream in the list and free the given Buffer
 */
void buffer_destroy(s_buffer *buf)
{
    if (!buf)
        return;
    for (int i = 0; i < buf->size; i++)
    {
        close(buf->buff[i]);
    }
    free(buf);
}


/*
 * Add a new file descriptor to the list
 * (Do nothing if the given Buffer or File descriptor is invalid)
 * (Delete the first entered element if the Buffer is full)
 */
void buffer_add(s_buffer *buf, FILE *file)
{
    if (!buf || !file)
    {
        return;
    }

    if (buf->size == BUF_SIZE)
    {
        buf->pos = (buf->pos + 1) % BUF_SIZE;
        close(buf->buff[buf->begin]);
        buf->begin = (buf->begin + 1) % BUF_SIZE;
    }
    else
    {
        buf->size += 1;
        buf->pos += 1;
    }
    buf->buff[buf->pos] = file;
}


/*
 * TODO
 */
FILE * buffer_undo(s_buffer *buf)
{
    FILE *file = buf->buff[buf->pos];
    buf->pos -= 1;
    if (buf->pos < 0)
        buf->pos += BUF_SIZE;
    return file;
}


/*
 * TODO
 */
FILE * buffer_redo(s_buffer *buf)
{
    FILE *file = buf->buff[buf->pos];
    buf->pos = (buf->pos + 1) % BUF_SIZE;
    return file;
}
