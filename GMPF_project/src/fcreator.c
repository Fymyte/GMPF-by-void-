#include "fcreator.h"

extern SGlobalData G_user_data;

/*
 * Save the newly created filter into a *.ftr file type as text
 * (Do nothing if the custom filter is incorect)
 */
void Save_filter()
{
    GdkRGBA color;

    GET_UI(GtkComboBoxText, effet_1, "effet1");
    GET_UI(GtkComboBoxText, effet_2, "effet2");


    gchar *filter_1 = gtk_combo_box_text_get_active_text (effet_1);
    gchar *filter_2 = gtk_combo_box_text_get_active_text (effet_2);

    GET_UI(GtkEntry, filename, "FilterFilename");
    GET_UI(GtkColorChooser, chooser, "FC_color");
    gchar *s = (gchar*)gtk_entry_get_text (filename);
    set_extension(&s, "ftr");

    int fd = creat(s, S_IRWXU);

    if (fd == -1)
    {
        PRINTERR("file creation failed\n");
        return;
    }
    //écrire le nom des fonctions
    //à appliquer dans le nouveau fichier en fonctions de l'utilisateur

    //write the filter_1 in the file
    if (strcmp(filter_1, "Grey") == 0 || strcmp(filter_2, "Grey") == 0)
    {
        if(write(fd, "Grey\n", 5) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize") == 0 || strcmp(filter_2, "Binarize") == 0)
    {
        if(write(fd, "Binarize\n", 9) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Blur") == 0 || strcmp(filter_2, "Blur") == 0)
    {
        if(write(fd, "Blur\n", 5) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Colorfull") == 0 || strcmp(filter_2, "Colorfull") == 0)
    {
        if(write(fd, "Colorfull\n", 10) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize color") == 0 || strcmp(filter_2, "Binarize color") == 0)
    {
        if(write(fd, "Binarize color\n", 15) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Tinter") == 0 || strcmp(filter_2, "Tinter") == 0)
    {
        if(write(fd, "Tinter\n", 15) == -1)
        {
            PRINTERR("write fail\n");
            return;
        }
    }

    //filter color save
    gtk_color_chooser_get_rgba(chooser, &color);
    gchar *savedColor = gdk_rgba_to_string (&color);
    size_t len = strlen(savedColor);

    if (write(fd, savedColor, len) == -1)
    {
        PRINTERR("write fail\n");
        return;
    }

    //close the filebt
    if(close(fd) == -1)
    {
        PRINTERR("close fail\n");
        return;
    }
    g_free(filter_1);
    g_free(filter_2);
    D_PRINT("Filter save done \n", NULL);
}


/*
 * Check that the extension of the filename is right ("lay"), return if not
 * Then open the file at filename and apply the custom filter to the selected
 * Layer
 * (Do nothing if there is no selected Layer)
 */
void Apply_user_filter(gchar *filename)
{
    if (!check_extension(filename, "lay"))
        return;
    FILE *filter = fopen(filename, "r");

    if (filter == NULL)
    {
        PRINTERR("file open fail\n");
        return;
    }

    char *save = malloc(35 * sizeof(char));
    char *line = NULL;

    while((line = fgets(save, 20, filter)) != NULL)
    {
        if (strcmp("Grey\n", save) == 0)
            GMPF_filter_apply_to_selected_layer(Greyscale);

        if (strcmp("Binarize\n", save) == 0)
            GMPF_filter_apply_to_selected_layer(Binarize);

        if (strcmp("Binarize color\n", save) == 0)
            GMPF_filter_apply_to_selected_layer(BinarizeColor);

        if (strcmp("Blur\n", save) == 0) //TODO
            GMPF_filter_apply_to_selected_layer(Greyscale);

        if (strcmp("Colorfull\n", save) == 0)
            GMPF_filter_apply_to_selected_layer(Colorfull);

        if (strcmp("Tinter\n", save) == 0) //TODO
            GMPF_filter_apply_to_selected_layer(Tinter);
    }

    if (fclose(filter) == -1)
    {
        PRINTERR("file close fail\n");
        return;
    }
    D_PRINT("user filter applied\n", NULL);
    free(save);
}
