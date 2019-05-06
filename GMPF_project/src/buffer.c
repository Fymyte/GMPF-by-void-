#include "buffer.h"


// #define CLOSE_BUFFER_FILES(_buffer, _begin, _end) \
//         if (_begin > _end) \
//         { \
//             for(; 0 < _end; _end--, _buffer->size--) \
//                 if (_buffer->elmt[_end]) \
//                     fclose(_buffer->elmt[_end]); \
//             if (_buffer->elmt[0]) \
//                 fclose(_buffer->elmt[0]); \
//             _end = BUFFER_SIZE - 1; \
//         } \
//         for(; _begin < _end; _end--, _buffer->size--) \
//             if (_buffer->elmt[_end]) \
//                 fclose(_buffer->elmt[_end]); \
//         if (_buffer->elmt[_begin]) \
//             fclose(_buffer->elmt[_begin]);




/*
 * Create a new Buffer and attach it to the given flowbox
 * (Return: the newly created Buffer, or NULL if it is unable to malloc)
 */
GMPF_Buffer *GMPF_buffer_init(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = buffer_create();

    if (!buffer)
    { PRINTERR ("No created buffer"); return NULL; }

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
    { PRINTERR("Unable to get buffer"); return; }

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
                    GMPF_Layer *layer)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);
    if (!buffer)
    { PRINTERR("Unable to get buffer"); return 1; }

    return buffer_add(buffer, action, layer);
}


int GMPF_buffer_undo(GtkFlowBox *flowbox)
{
    GMPF_Buffer *buffer = GMPF_buffer_get_buffer(flowbox);
    if (!buffer)
    {
        PRINTERR("Unable to get buffer");
        return 1;
    }

    if (buffer_undo(buffer, flowbox))
    { PRINTERR("Unable to undo the last action"); return 1; }

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

    if (buffer_redo(buffer, flowbox))
    { PRINTERR("Unable to redo the last action"); return 1; }

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

    if (buffer_init(buffer))
    { PRINTERR("Unable to malloc"); }

    return buffer;
}


/*
 * Set all parameters from the given buffer to there default values
 * (Do nothing if the given buffer is invalid)
 * (Return: 0 if there were no error, else 1)
 */
char buffer_init(GMPF_Buffer *buffer)
{
    if (!buffer)
    {
        PRINTERR ("Invalid buffer");
        return 1;
    }
    buffer->begin = 0;
    buffer->end = 0;
    buffer->size = 0;
    buffer->pos = 0;
    for (size_t i = 1; i < BUFFER_SIZE; i++)
    {
        buffer->elmt[i] = NULL;
    }
    buffer->elmt[0] = tmpfile();
    return 0;
}


/*
 * Close all open filestream in the list and free the given Buffer
 * (Do nothing if the given buffer is invalid)
 */
char buffer_destroy(GMPF_Buffer *buffer)
{
    if (!buffer)
    { PRINTERR("Invalid buffer"); return 1; }

    for(; buffer->begin <= buffer->end; buffer->begin++)
    { fclose(buffer->elmt[buffer->begin % BUFFER_SIZE]); }

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
                GMPF_Action  action,
                GMPF_Layer  *layer)
{
    if (!buffer || !layer)
    {
        PRINTERR("Invalid buffer or layer");
        return 1;
    }

    FILE *file = tmpfile();
    if (!file)
    { PRINTERR("No file"); return 1; }

    s_savebuf save;
    save.layer = gtk_flow_box_child_get_index(layer->UIElement);
    save.action = action;
    if (fwrite(&save, sizeof(s_savebuf), 1, file) != 1)
    { PRINTERR("Unable to write in filestream"); return 1; }

    switch (action) {
        case GMPF_ACTION_MOVE_UP:
            break;

        case GMPF_ACTION_MOVE_DOWN:
            break;

        case GMPF_ACTION_MODIF_IMAGE:
            if (save_layer(layer, file))
            { PRINTERR("Unable to save layer"); break; }
            break;

        case GMPF_ACTION_CHANGE_NAME:
            if (fwrite(&layer->name, sizeof(char), 51, file) != 51)
            { PRINTERR("Unable to write in filestream"); break; }
            break;

        case GMPF_ACTION_DELETE:
            if (save_layer(layer, file))
            { PRINTERR("Unable to save layer"); break; }
            break;

        case GMPF_ACTION_ADD:
            if (save_layer(layer, file))
            { PRINTERR("Unable to save layer"); break; }
            break;

        default:
            PRINTERR("Unknown action");
    }

    for(; buffer->end > buffer->pos; buffer->end--, buffer->size--)
    {
        fclose(buffer->elmt[buffer->end % BUFFER_SIZE]);
    }

    buffer->size = buffer->end - buffer->begin + 1;

    if (buffer->size == BUFFER_SIZE)
    {
        fclose(buffer->elmt[buffer->begin % BUFFER_SIZE]);
        buffer->begin++;
    }
    else
    {
        buffer->size++;
    }

    buffer->pos++;
    buffer->end++;
    rewind(file); // Set the position in the file to 0
    buffer->elmt[buffer->end % BUFFER_SIZE] = file;



    D_PRINT("buffer -- pos: %llu, begin: %llu, end: %llu, size: %llu",
            buffer->pos, buffer->begin, buffer->end, buffer->size);
    return 0;
}


/*
 * TODO
 */
char buffer_undo(GMPF_Buffer *buffer,
                 GtkFlowBox  *flowbox) // TODO
{
    if (!buffer->size || buffer->pos <= buffer->begin)
    { return 1; }

    FILE *file = buffer->elmt[buffer->pos % BUFFER_SIZE];
    if (!file)
    { PRINTERR("No file"); return 1; }
    s_savebuf s_buff;
    if (fread(&s_buff, sizeof(s_savebuf), 1, file) != 1)
    { PRINTERR ("Unable to read in filestream"); return 1; }

    GMPF_Action action = s_buff.action;
    GMPF_Layer *layer = NULL;

    switch (action) {
        case GMPF_ACTION_MOVE_UP:
            layer = layer_get_at_pos(flowbox, s_buff.layer - 1);
            if (!list_move_down(&layer->list))
            { PRINTERR("Unable to move up in list"); break; }
            gtk_widget_destroy((GtkWidget *) layer->UIElement);
            layer_insert_at_pos(layer, flowbox, s_buff.layer);
            gtk_flow_box_select_child(flowbox, layer->UIElement);
            break;

        case GMPF_ACTION_MOVE_DOWN:
            layer = layer_get_at_pos(flowbox, s_buff.layer + 1);
            if (!list_move_up(&layer->list))
            { PRINTERR("Unable to move up in list"); break; }
            gtk_widget_destroy((GtkWidget *) layer->UIElement);
            layer_insert_at_pos(layer, flowbox, s_buff.layer);
            gtk_flow_box_select_child(flowbox, layer->UIElement);
            break;

        case GMPF_ACTION_MODIF_IMAGE:
            if (load_layer(layermngr_get_layermngr(flowbox), file))
            { PRINTERR("Unable to load layer"); break; }

            layer = layer_get_at_pos(flowbox, s_buff.layer + 1);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            rewind(file);
            if (save_layer(layer, file))
            { PRINTERR("Unable to save layer"); break; }

            layer_delete(layer);

            layer = layer_get_at_pos(flowbox, 0);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }
            for (int i = 0; i < s_buff.layer; i++)
            {
                if (!list_move_down(&layer->list))
                { PRINTERR("Unable to move down"); }
            }
            break;

        case GMPF_ACTION_CHANGE_NAME:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            char name[51];
            if (fread(&name, sizeof(char), 51, file) != 51)
            { PRINTERR("Unable to read in filestream"); break; }

            rewind(file);
            if (fwrite(&layer->name, sizeof(char), 51, file) != 51)
            { PRINTERR("Unable to write in filestream"); break; }

            for (int i = 0; i < 51; i++)
            { layer->name[i] = name[i]; }
            break;

        case GMPF_ACTION_DELETE:
            if (load_layer(layermngr_get_layermngr(flowbox), file))
            { PRINTERR("Unable to load layer"); break; }

            layer = layer_get_at_pos(flowbox, 0);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }
            for (int i = 0; i < s_buff.layer; i++)
            {
                if (!list_move_down(&layer->list))
                { PRINTERR("Unable to move down"); }
            }
            break;

        case GMPF_ACTION_ADD:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            layer_delete(layer);
            break;

        default: break;
    }
    buffer->pos--;
    rewind(file); // Set the positon in the file to 0
    D_PRINT("buffer -- pos: %llu, begin: %llu, end: %llu, size: %llu",
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
char buffer_redo(GMPF_Buffer *buffer,
                 GtkFlowBox  *flowbox) // TODO
{
    if (!buffer->size || buffer->pos >= buffer->end)
    { return 1; }

    buffer->pos++;

    FILE *file = buffer->elmt[buffer->pos % BUFFER_SIZE];
    if (!file)
    { PRINTERR("No file"); return 1; }

    s_savebuf s_buff;
    if (fread(&s_buff, sizeof(s_savebuf), 1, file) != 1)
    { PRINTERR ("Unable to read in filestream"); return 1; }

    GMPF_Action action = s_buff.action;
    GMPF_Layer *layer = NULL;

    switch (action) {
        case GMPF_ACTION_MOVE_UP:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!list_move_up(&layer->list))
            { PRINTERR("Unable to move up in list"); break; }
            gtk_widget_destroy((GtkWidget *) layer->UIElement);
            layer_insert_at_pos(layer, flowbox, s_buff.layer - 1);
            gtk_flow_box_select_child(flowbox, layer->UIElement);
            break;

        case GMPF_ACTION_MOVE_DOWN:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!list_move_down(&layer->list))
            { PRINTERR("Unable to move up in list"); break; }
            gtk_widget_destroy((GtkWidget *) layer->UIElement);
            layer_insert_at_pos(layer, flowbox, s_buff.layer + 1);
            gtk_flow_box_select_child(flowbox, layer->UIElement);
            break;

        case GMPF_ACTION_MODIF_IMAGE:
            if (load_layer(layermngr_get_layermngr(flowbox), file))
            { PRINTERR("Unable to load layer"); break; }

            layer = layer_get_at_pos(flowbox, s_buff.layer + 1);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            rewind(file);
            if (save_layer(layer, file))
            { PRINTERR("Unable to save layer"); break; }

            layer_delete(layer);

            layer = layer_get_at_pos(flowbox, 0);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }
            for (int i = 0; i < s_buff.layer; i++)
            {
                if (!list_move_down(&layer->list))
                { PRINTERR("Unable to move down"); }
            }
            break;

        case GMPF_ACTION_CHANGE_NAME:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            char name[51];
            if (fread(&name, sizeof(char), 51, file) != 51)
            { PRINTERR("Unable to read in filestream"); break; }

            rewind(file);
            if (fwrite(&layer->name, sizeof(char), 51, file) != 51)
            { PRINTERR("Unable to write in filestream"); break; }

            for (int i = 0; i < 51; i++)
            { layer->name[i] = name[i]; }
            break;

        case GMPF_ACTION_DELETE:
            layer = layer_get_at_pos(flowbox, s_buff.layer);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }

            layer_delete(layer);
            break;

        case GMPF_ACTION_ADD:
            if (load_layer(layermngr_get_layermngr(flowbox), file))
            { PRINTERR("Unable to load layer"); break; }

            layer = layer_get_at_pos(flowbox, 0);
            if (!layer)
            { PRINTERR("Unable to get layer"); break; }
            for (int i = 0; i < s_buff.layer; i++)
            {
                if (!list_move_down(&layer->list))
                { PRINTERR("Unable to move down"); }
            }
            break;

        default: break;
    }

    rewind(file);

    D_PRINT("buffer -- pos: %llu, begin: %llu, end: %llu, size: %llu",
            buffer->pos, buffer->begin, buffer->end, buffer->size);

    return 0;
}
