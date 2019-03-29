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
    if (strcmp(filter_1, "Grey") == 0)
    {
        if(write(fd, "Grey\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize") == 0)
    {
        if(write(fd, "Binarize\n", 9) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Blur") == 0)
    {
        if(write(fd, "Blur\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Colorfull") == 0)
    {
        if(write(fd, "Colorfull\n", 10) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_1, "Binarize color") == 0)
    {
        if(write(fd, "Binarize color\n", 15) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    //write the filter_2 in the file
    if (strcmp(filter_2, "Grey") == 0)
    {
        if(write(fd, "Grey\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_2, "Binarize") == 0)
    {
        if(write(fd, "Binarize\n", 9) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_2, "Blur") == 0)
    {
        if(write(fd, "Blur\n", 5) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_2, "Colorfull") == 0)
    {
        if(write(fd, "Colorfull\n", 10) == -1)
        {
            printf("write fail\n");
            return;
        }
    }

    if (strcmp(filter_2, "Binarize color") == 0)
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
    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        printf("file open fail\n");
        return;
    }

    void *buffer = calloc(35, sizeof(char));
    int r = read(fd, buffer, 35);

    if (strncmp("Grey", buffer, r) == 0)
        Greyscale(data);

    if (strncmp("Binarize", buffer, r) == 0)
        Binarize(data);

    if (strncmp("Binarize color", buffer, r) == 0)
        BinarizeColor(data);

    if (strncmp("Colorfull", buffer, r) == 0)
        Colorfull(data);

    if (strncmp("Blur", buffer, r) == 0) //TODO
        Greyscale(data);

    while(r > 0)
    {
        if (strncmp("Grey", buffer, r) == 0)
            Greyscale(data);

        if (strncmp("Binarize", buffer, r) == 0)
            Binarize(data);

        if (strncmp("Binarize color", buffer, r) == 0)
            BinarizeColor(data);

        if (strncmp("Colorfull", buffer, r) == 0)
            Colorfull(data);

        if (strncmp("Blur", buffer, r) == 0) //TODO
            Greyscale(data);

        r = read(fd, buffer, 35);
    }

    if (close(fd) == -1)
    {
        printf("file close fail\n");
        return;
    }
    printf("user filter applied\n");
}
