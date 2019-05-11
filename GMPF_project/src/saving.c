#include "saving.h"

extern SGlobalData G_user_data;

/*
 * Return the extention of the given filename
 * (Return: the extension, or NULL, if there is no extension or filename is invalid)
 */
char *get_extension(char *filename)
{
    if (!filename)
        return NULL;
    char *c = NULL;
    for ( ; *filename != '\0'; filename++)
    {
        if (*filename == '.')
        {
            c = filename;
        }
    }

    return c == NULL || c + 1 == NULL ? NULL : c + 1;
}


/*
 * Check if the extension of "filename" is right "extension"
 * (Return: 1 if the extension is correct, else 0)
 */
int check_extension(char *filename, char *extension)
{
    char *c = NULL;
    for ( ; *filename != '\0'; filename++)
    {
        if (*filename == '.')
        {
            c = filename;
        }
    }

    if (c == NULL || !strcmp(c + 1, extension))
        return 0;
    return 1;
}


/*
 * Set the extention of the given filename to "extension"
 * (Return: 0 if there is no error, else -1)
 */
int set_extension(char **filename, char *extension)
{
    if (!check_extension(*filename, extension))
        return 0;
    D_PRINT("check is false", NULL);
    size_t filelen = strlen(*filename) + strlen(extension) + 3;
    char *newfilename = malloc(sizeof(char) * filelen);
    if (newfilename == NULL)
    {
        PRINTERR ("Unable to malloc the new filename");
        return -1;
    }
    sprintf(newfilename, "%s.%s", *filename, extension);
    free(*filename);
    *filename = newfilename;
    return 0;
}



/*
 * PURPOSE : Return the name of the file in Filename
 *  PARAMS : char *filename - the filename witch contain the name of file
 * RETURNS : char *- the name of the file
 *   NOTES : return NULL if filename is incorrect
 */
char *filename_get_name_of_file(char *filename)
{
    if (!filename)
        return NULL;
    char *c = NULL;
    for ( ; *filename != '\0'; filename++)
    {
        if (*filename == '/')
        {
            c = filename;
        }
    }

    return c == NULL || c + 1 == NULL ? NULL : c + 1;
}


/*
 * Save the given LayerMngr in File "file"
 * (Return: 0 if there is no error, else 1)
 */
char save_layermngr(GMPF_LayerMngr *layermngr, FILE *file)
{
    if (fwrite(layermngr, sizeof(GMPF_LayerMngr), 1, file) != 1)
        return 1;
    return 0;
}


/*
 * Load the LayerMngr saved in the File "file" and associat it to the given
 * flowbox
 * (Return: 0 if there is no error, else 1)
 */
char load_layermngr(GtkFlowBox *flowbox, FILE *file)
{
    GMPF_LayerMngr *layermngr = malloc(sizeof(GMPF_LayerMngr));
    if (layermngr == NULL) { PRINTERR("Unable to malloc"); return 1; }
    if (fread(layermngr, sizeof(GMPF_LayerMngr), 1, file) != 1)
    { PRINTERR ("Unable to read LayerMngr in filestream"); return 1; }
    list_init(&(layermngr->layer_list));

    layermngr->surface = NULL;
    // NEED TO INIT THE CAIRO SURFACE

    layermngr->image = NULL;
    layermngr->display_image = NULL;
    layermngr->filename = NULL;

    layermngr_set_to_flowbox(flowbox, layermngr);
    return 0;
}


/*
 * Save the given Layer in File "file"
 * (Return: 0 if there is no error, else 1)
 */
char save_layer(GMPF_Layer *layer, FILE *file)
{
    if (fwrite(layer, sizeof(GMPF_Layer), 1, file) != 1)
     { PRINTERR ("Unable to write in filestream"); return 1; }

    //if (fwrite(layer->image, sizeof(GdkPixbuf), 1, file) != 1)
    //    return 1;
    unsigned int rowstride = (unsigned int) gdk_pixbuf_get_rowstride(layer->image);
    unsigned int length = layer->size.h * rowstride;
    guchar *image = gdk_pixbuf_get_pixels(layer->image);
    if (fwrite(&length, sizeof(unsigned int), 1, file) != 1)
    { PRINTERR ("Unable to write in filestream"); return 1; }
    if (fwrite(image, length, 1, file) != 1)
    { PRINTERR ("Unable to write in filestream"); return 1; }

    return 0;
}


/*
 * Load the Layer saved in the File "file" and associat it to the given flowbox
 * (Return: 0 if there is no error, else 1)
 */
char load_layer(GMPF_LayerMngr *layermngr, FILE *file)
{
    GMPF_Layer *layer = malloc(sizeof(GMPF_Layer));
    if (layer == NULL) { PRINTERR ("Unable to malloc"); return 1; }

    GtkFlowBox *flowbox = layermngr->flowbox;
    if (flowbox == NULL) { PRINTERR ("Unable to get flowbox"); return 1; }

    if (fread(layer, sizeof(GMPF_Layer), 1, file) != 1)
    { // restore the layer
        PRINTERR ("Unable to read in filestream");
        free(layer);
        return 1;
    }
    unsigned int length;
    if (fread(&length, sizeof(unsigned int), 1, file) != 1)
    {
        PRINTERR ("Unable to read in filestream");
        free(layer);
        return 1;
    }

    unsigned int true_length = ((unsigned int) layer->size.w * ((unsigned int) layer->size.h << 2));
    if (length != true_length)
    { // test if the length of the pixmap is good
        PRINTERR ("length and true_length doesn't correspond");
        free(layer);
        return 1;
    }

    guchar *data = malloc(length * sizeof(guchar));
    if (fread(data, length, 1, file) != 1)
    {
        PRINTERR ("Unable to read in filestream");
        free(data);
        free(layer);
        return 1;
    }

    layer->image = gdk_pixbuf_new_from_data (data,
                           GDK_COLORSPACE_RGB, TRUE, 8,
                           layer->size.w, layer->size.h,
                           layer->size.w << 2,
                       NULL, NULL);

    layer->surface = gdk_cairo_surface_create_from_pixbuf(layer->image, 0, NULL);
    layer->scale_factor.x = 1;
    layer->scale_factor.y = 1;

    GtkWidget *image = gtk_image_new();

    // Style of the image
    INIT_LAYER_UI(layer, image, grid);

    /*int insertpos;*/
    // add the layer in the list
    list_append(&(layermngr->layer_list), &(layer->list));
    /*insertpos = 0;*/

    gtk_flow_box_insert (flowbox, grid, 0 /*insertpos*/);

    layer->UIElement =
        gtk_flow_box_get_child_at_index(flowbox, 0 /*insertpos*/);
    g_object_set_data(G_OBJECT(layer->UIElement), LAYER_KEY_NAME, layer);

    layer->UIIcon = (GtkGrid *) grid;

    layer->cr = NULL;
    layer->icon = NULL;
    layer->filename = NULL;
    layer_icon_refresh(layer);


    return 0;
}


/*
 * Save the project at the given filename
 * (Return: 0 if there is no error, else 1)
 */
char save_project(GtkFlowBox *flowbox, const char *filename)
{
    // write the file / delete it if it already exists
    //FILE *tmpFile = tmpfile(void);

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL) { PRINTERR ("Unable to get LayerMngr"); return 1; }

    FILE *file = fopen(filename, "wb"); // write as binary => rb
    if (file == NULL) { PRINTERR ("Unable to open filestream"); return 1; }

    char err = save_layermngr(layermngr, file);

    if (err)
    {
        if (fclose(file)) { PRINTERR("Unable to close filestream"); }
        PRINTERR("Unable to save LayerMngr");
        return 1;
    }

    GMPF_List *list = layermngr->layer_list.next;
    GMPF_Layer *lay = container_of(list, GMPF_Layer, list);
    for (int i = 0; i < layermngr->nb_layer; i++)
    {
        err = save_layer(lay, file);
        if (err)
        {
            if (fclose(file)) { PRINTERR ("Unable to close filestream")};
            PRINTERR ("Unable to save Layer");
            return 1;
        }
        lay = container_of(lay->list.next, GMPF_Layer, list);
    }

    long long dbg = 0;
    fwrite(&dbg, sizeof(long long), 1, file);

    fclose(file);
    return 0;
}


/*
 * Load the project at the given filename and associate it to the given flowbox
 * (Return: 0 if there is no error, else 1)
 */
char load_project(GtkFlowBox *flowbox,
                  const char *filename)
{
    // only read the file
    FILE *file = fopen(filename, "rb"); // read as binary => rb
    if (file == NULL) { PRINTERR ("Unable to open filestream"); return 1; }

    if (g_object_get_data(G_OBJECT(flowbox), LAYERMNGR_KEY_NAME) != NULL)
        layermngr_delete(flowbox);

    char err = load_layermngr(flowbox, file);
    if (err)
    {
        if (fclose(file)) { PRINTERR ("Unable to close filestream"); }
        PRINTERR ("Unable to load LayerMngr");
        return 1;
    }

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL)
    {
        if (fclose(file)) { PRINTERR("Unable to close filestream"); }
        PRINTERR ("Unable to get LayerMngr");
        return 1;
    }

    for (int i = 0; i < layermngr->nb_layer; i++)
    {
        err = load_layer(layermngr, file);
        if (err)
        {
            if (fclose(file)) { PRINTERR("Unable to close filestream"); }
            PRINTERR ("Unable to load Layer");
            return 1;
        }
    }

    fclose(file);
    return 0;
}


/*
 * Save the selected Layer at the given filename
 * (Do nothing if there is no selected Layer)
 * (Return: 0 if there is no error, 1 else of there is no selected Layer)
 */
char saving_layer(GtkFlowBox *flowbox, const char *filename)
{
    // write the file / delete it if it already exists
    //FILE *tmpFile = tmpfile(void)

    FILE *file = fopen(filename, "wb"); // write as binary => rb
    if (file == NULL) { PRINTERR ("Unable to open filestream"); return 1; }

    GMPF_Layer *lay = layermngr_get_selected_layer(flowbox);
    if (!lay)
        return 1;
    char err = save_layer(lay, file);
    if (err)
    {
        if (fclose(file)) { PRINTERR("Unable to close filestream"); }
        PRINTERR ("Unable to save Layer");
        return 1;
    }

    long long dbg = 0;
    fwrite(&dbg, sizeof(long long), 1, file);

    fclose(file);
    return 0;
}


/*
 * Load the Layer at the given filename and append it to the Layer list
 * (Return: 0 if there is no error, else 1)
 */
char loading_layer(GtkFlowBox *flowbox, const char *filename)
{
    // only read the file
    FILE *file = fopen(filename, "rb"); // read as binary => rb
    if (file == NULL) { PRINTERR ("Unable to open filestream"); return 1; }

    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);
    if (layermngr == NULL)
    {
        if (fclose(file)) { PRINTERR("Unable to close filestream"); }
        PRINTERR ("Unable to get LayerMngr");
        return 1;
    }

    char err = load_layer(layermngr, file);
    if (err)
    {
        if (fclose(file)) { PRINTERR("Unable to close filestream"); }
        PRINTERR ("Unable to load Layer");
        return 1;
    }

    fclose(file);
    return 0;
}


/*
 * Export the project to a PNG image file at the given filename
 * (Return: 0 if there is no error, else -1)
 */
int export_cairo_to_png(gchar *filename, GtkFlowBox *flowbox)
{
    GMPF_LayerMngr *layermngr = layermngr_get_layermngr(flowbox);

    if (layermngr->layer_list.next == NULL)
        return -1; //save failed

	GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);

	//create a new surface and context to store all the layer surfaces
	cairo_surface_t *final_surface =
		cairo_image_surface_create (CAIRO_FORMAT_ARGB32, layermngr->size.w, layermngr->size.h);
	cairo_t *final_context = cairo_create(final_surface);

	//write the surfaces on the new surface
    while (lay != NULL)
    {
        if (lay->isvisible)
        {
            cairo_save(final_context);
            // cairo_surface_t *surface = lay->surface ? lay->surface : lay->surface;
            // cairo_scale(final_context, lay->scale_factor.x, lay->scale_factor.y);
            cairo_set_source_surface (final_context, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
            cairo_paint(final_context);
            cairo_restore(final_context);
        }
        if (!lay->list.next) break;
        lay = container_of(lay->list.next, GMPF_Layer, list);
    }

	//add th epng extension

	//get the surface from the context and save it
    final_surface = cairo_get_target(final_context);
    cairo_status_t status = cairo_surface_write_to_png (final_surface, filename);
    cairo_destroy(final_context);
    cairo_surface_destroy(final_surface);

    if (status == CAIRO_STATUS_NO_MEMORY)
    {
        printf("%s\n", cairo_status_to_string(status));
        return -1;
    }

    else if (status == CAIRO_STATUS_WRITE_ERROR)
    {
        printf("%s\n", cairo_status_to_string(status));
        return -1;
    }

    else if (status == CAIRO_STATUS_READ_ERROR)
    {
        printf("%s\n", cairo_status_to_string(status));
        return -1;
    }
    return 0;
}
