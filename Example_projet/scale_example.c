//#include <cairo.h>
#include <gtk-3.0/gtk/gtk.h>
#include <cairo.h>


static gboolean
on_expose_event
(
        GtkWidget *widget,
        cairo_t *cr,
        gpointer data)
{
        //cairo_t *cr;

        //cr = gdk_cairo_create (widget->window);
        cairo_save (cr);
        cairo_set_source_rgb (cr, 0.6, 0.6, 0.6);
        cairo_rectangle (cr, 20, 30, 80, 50);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);
        cairo_restore (cr);
        cairo_save (cr);
        cairo_translate (cr, 130, 30);
        cairo_scale (cr, 0.7, 0.7);
        cairo_set_source_rgb (cr, 0.6, 0.6, 0.6);
        cairo_rectangle (cr, 0, 0, 80, 50);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);
        cairo_restore (cr);
        cairo_save (cr);
        cairo_translate (cr, 220, 30);
        cairo_scale (cr, 1.5, 1.5);
        cairo_set_source_rgb (cr, 0.6, 0.6, 0.6);
        cairo_rectangle (cr, 0, 0, 80, 50);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);
        cairo_restore (cr);
        cairo_destroy (cr);
        return FALSE;
}


int
main (int argc, char *argv[])
{
        GtkWidget *window;
        GtkWidget *da;

        gtk_init (&argc, &argv);
        da = gtk_drawing_area_new();
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_container_add((GtkContainer *)window, da);
        g_signal_connect(da, "draw",
                G_CALLBACK (on_expose_event), NULL);
        g_signal_connect(window, "destroy",
                G_CALLBACK (gtk_main_quit), NULL);
        gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size (GTK_WINDOW (window), 360, 140);
        //gtk_widget_set_app_paintable (window, TRUE);
        gtk_widget_show_all (window);
        gtk_main ();
        return 0;
}
