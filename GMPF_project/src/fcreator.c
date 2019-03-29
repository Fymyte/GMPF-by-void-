#include "fcreator.h"

void Save_filter(gpointer user_data)
{
    INIT_UI();
    GdkRGBA color;

    GET_UI(GtkComboBoxText, effet_1, "effet1");
    GET_UI(GtkComboBoxText, effet_2, "effet2");


    gchar *filter_1 = gtk_combo_box_text_get_active_text (effet_1);
    gchar *filter_2 = gtk_combo_box_text_get_active_text (effet_2);

    GET_UI(GtkEntry, filename, "FilterFilename");
    GET_UI(GtkColorChooser, chooser, "FC_color");
    gchar *s = (gchar*)gtk_entry_get_text (filename);
    s = strcat(s, ".txt");

    int fd = creat(s, S_IRWXU);

    if (fd == -1)
    {
        printf("file creation failed\n");
        return;
    }
    //écrire le nom des fonctions
    //à appliquer dans le nouveau fichier en fonctions de l'utilisateur

    //write the filter_1 in the file
    if (strcmp(filter_1, "Grey") == 0 || strcmp(filter_2, "Grey") == 0)
    {
        if(write(fd, "Grey\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize") == 0 || strcmp(filter_2, "Binarize") == 0)
    {
        if(write(fd, "Binarize\n", 9) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Blur") == 0 || strcmp(filter_2, "Blur") == 0)
    {
        if(write(fd, "Blur\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Colorfull") == 0 || strcmp(filter_2, "Colorfull") == 0)
    {
        if(write(fd, "Colorfull\n", 10) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize color") == 0 || strcmp(filter_2, "Binarize color") == 0)
    {
        if(write(fd, "Binarize color\n", 15) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    //filter color save
    gtk_color_chooser_get_rgba(chooser, &color);
    gchar *savedColor = gdk_rgba_to_string (&color);
    size_t len = strlen(savedColor);

    if (write(fd, savedColor, len) == -1)
    {
        printf("write fail\n");
        return;
    }

    //close the filebt
    if(close(fd) == -1)
    {
        printf("close fail\n");
        return;
    }
    g_free(filter_1);
    g_free(filter_2);
    printf("Filter save done \n");
}

void Apply_user_filter(gchar *filename, gpointer user_data)
{
    SGlobalData *data = (SGlobalData *)user_data;
    FILE *filter = fopen(filename, "r");

    if (filter == NULL)
    {
        printf("file open fail\n");
        return;
    }

    char *save = malloc(35 * sizeof(char));
    char *line = NULL;

    while((line = fgets(save, 20, filter)) != NULL)
    {
        printf("%s.\n", line);
        if (strcmp("Grey\n", save) == 0)
            Greyscale(data);

        if (strcmp("Binarize\n", save) == 0)
            Binarize(data);

        if (strcmp("Binarize color\n", save) == 0)
            BinarizeColor(data);

        if (strcmp("Colorfull\n", save) == 0)
            Colorfull(data);

        if (strcmp("Blur\n", save) == 0) //TODO
            Greyscale(data);

        if (strcmp("Tinter\n", save) == 0) //TODO
            Greyscale(data);
    }

    if (fclose(filter) == -1)
    {
        printf("file close fail\n");
        return;
    }
    printf("user filter applied\n");
    free(save);
}
