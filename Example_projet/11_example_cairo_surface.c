#include <gtk/gtk.h>

typedef struct
{
  GtkBuilder *builder;
  GList *calclist;
} SGlobalData;

typedef struct
{
  cairo_surface_t *surface;
  GdkRGBA color;
} SCalc;


gboolean
on_drawingarea_draw (GtkWidget *widget, cairo_t *cr, SGlobalData *gui)
{
  gint width, height;
  static gboolean firstaccess = TRUE; // Permet de configurer les calques à la première exécution de cette fonction

  // Récupération de la taille de la surface de fond
  gtk_widget_get_size_request (widget, &width, &height);

  // Affichage de l'image de fond (fond noir avec texte en blanc "image de fond")
  cairo_paint (cr);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_move_to (cr, 0, 15);
  cairo_set_font_size (cr, 20);
  cairo_show_text (cr, "Image de fond");

  // Création si besoin des différents calques
  if (firstaccess)
    {
      gint i, j=25;
      gchar *text=NULL;
      cairo_t *newcr = NULL;
      SCalc *calc = NULL;

      for (i=0; i<4; i++)
	{
	  // Déclaration d'un nouveau calque
	  calc = g_new (SCalc, 1);

	  calc->surface = cairo_surface_create_similar (cairo_get_target (cr), CAIRO_CONTENT_COLOR_ALPHA, width, height);
	  calc->color.red = i*0.2;
	  calc->color.green = 1;
	  calc->color.blue = i*0.3;
	  calc->color.alpha = 1;

	  // Création d'un nouveau context en fonction de la nouvelle surface
	  newcr = cairo_create (calc->surface);

	  // Ecriture en blanc dans la nouvelle surface "calque n°x"
	  cairo_set_source_rgb (newcr, calc->color.red, calc->color.green, calc->color.blue);
	  cairo_move_to (newcr, j-15, j);
	  text = g_strdup_printf ("calque n°%d", i+1);
	  cairo_set_font_size (newcr, 20);
	  cairo_show_text (newcr, text);
	  g_free (text);

	  // Ajout du nouveau calque à la fin de la liste
	  gui->calclist = g_list_append (gui->calclist, calc);

	  // Suppression du context devenu inutile
	  cairo_destroy (newcr);
	  j+=10;
	}

      firstaccess = FALSE;
    }

  // Affichage de tous les claques
  GList *list = gui->calclist;
  while (list)
    {
      cairo_set_source_surface (cr, ((SCalc*)(list->data))->surface, 0, 0);
      cairo_paint (cr);
      list = g_list_next(list);
    }

  return FALSE;
}

gint
main (gint argc, gchar *argv[])
{
  SGlobalData gui;
  GtkWidget *window=NULL;
  GtkWidget *drawingarea = NULL;

  gui.calclist = NULL;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  drawingarea = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawingarea, 200, 200);
  gtk_container_add (GTK_CONTAINER (window), drawingarea);

  g_signal_connect (G_OBJECT (window), "destroy", (GCallback) gtk_main_quit, NULL);
  g_signal_connect (G_OBJECT (drawingarea), "draw", (GCallback) on_drawingarea_draw, &gui);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
