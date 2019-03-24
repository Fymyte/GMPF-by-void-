#include <assert.h>
#include <gtk/gtk.h>

#define CALL_BACK_EVERY_MS 50
// used to decide if to use gtk_widget_queue_draw
int withWidgetQueueDraw = 0;


/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

void print_event(GtkWidget *widget, GdkEventMotion *event, gpointer p)
{
    printf("%.0f, %.0f: %d\n", event->x, event->y, event->time);
}


static void
clear_surface (void)
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
     GtkAllocation allocation;
    if (surface)
        cairo_surface_destroy (surface);

     gtk_widget_get_allocation (widget, &allocation);

    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                 CAIRO_CONTENT_COLOR,
                                                 allocation.width,
                                                 allocation.height);

    /* Initialize the surface to white */
    clear_surface ();

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{

    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

static gboolean
time_handler(GtkWidget *widget)
{
    if (gtk_widget_get_window (widget) == NULL) return FALSE;


    if (withWidgetQueueDraw )
        ; // do nothing, handled by draw_line
    else {
        cairo_t   *cr;
        cr = gdk_cairo_create(gtk_widget_get_window (widget));

        cairo_set_source_surface (cr, surface, 0, 0);
        cairo_paint (cr);
    }
    return TRUE;
}


/* Draw a rectangle on the surface at the given position */
static void
draw_line (GtkWidget *widget,
           gdouble    x,
           gdouble    y,
           gdouble    xOld,
           gdouble    yOld
           )
{
    cairo_t *cr;
    static double color=0;
    static double color2=0;
    static int alternate = 0;
    color += 0.1;
    if (color > 0.8)
        color = 0;
    color2 += 0.2;
    if (color2 > 0.8)
        color2 = 0;
    alternate = !alternate;

    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);
    cairo_new_path(cr);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

    cairo_set_line_width(cr,3);

    if (alternate) {
        if (withWidgetQueueDraw) {
            cairo_set_source_rgba(cr, color, color2,0.0, 1.0);
        } else {
            cairo_set_source_rgba(cr, color, color2,0.9, 1.0);
        }
    } else {
        cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 1.0);
    }

    cairo_move_to(cr, xOld, yOld);
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */

    if (withWidgetQueueDraw) {
        // find rectangle to invalidate
        int ax, ay, bx, by;
        if (x < xOld) {
            ax = x;
            bx = xOld;
        } else {
            bx = x;
            ax = xOld;
        }
        if (y < yOld) {
            ay = y;
            by = yOld;
        } else {
            by = y;
            ay = yOld;
        }
        assert(ax <= bx);
        assert(ay <= by);
        gtk_widget_queue_draw_area (widget, ax-1, ay-1, bx+1, by+1);
    }

}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean
button_press_event_cb (GtkWidget      *widget,
                       GdkEventButton *event,
                       gpointer        data)
{
    /* paranoia check, in case we haven't gotten a configure event */
    if (surface == NULL)
        return FALSE;

    if (event->button == 1){
        // handled by draw line. It ignores first point for simplicity
    } else if (event->button == 2) {
        withWidgetQueueDraw = !withWidgetQueueDraw;
    }
    else if (event->button == 3) {
        clear_surface ();
        gtk_widget_queue_draw (widget);
    }

    /* We've handled the event, stop processing */
    return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean
motion_notify_event_cb (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
    static int prevX=-1;
    static int prevY=-1;
    int x, y;

    GdkModifierType state;

    /* paranoia check, in case we haven't gotten a configure event */
    if (surface == NULL)
        return FALSE;

    x = event->x;
    y = event->y;
    state = event->state;
    //printf("event %f %f %d %d\n", event->x, event->y, x, y);

    if (prevX > 0 && state & GDK_BUTTON1_MASK) {
        //         printf("hello [%d][%d]->[%d][%d]\n",prevX, prevY,x,y);

        draw_line (widget, x,y,prevX, prevY);

    }
    prevX = x;
    prevY = y;
    /* We've handled it, stop processing */
    return TRUE;
}



static void
close_window (void)
{
    if (surface)
        cairo_surface_destroy (surface);

    gtk_main_quit ();
}

int
main (int   argc,
      char *argv[])
{
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *da;

    printf("Test for drawing\n\n\tBlue-ish lines are drawn using a callback every %d milliseconds\n\n\tGreen-ish lines are drown using gtk_widget_queue_draw at every event\n\nPress left button to draw, middle button to switch drawing mode, right button to clear\n\n", CALL_BACK_EVERY_MS );


    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    frame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (window), frame);

    da = gtk_drawing_area_new ();


    /* set a minimum size */
    gtk_widget_set_size_request (da, 1000, 700);

    gtk_container_add (GTK_CONTAINER (frame), da);

    /* Signals used to handle the backing surface */
    g_signal_connect (da, "draw",
                      G_CALLBACK (draw_cb), NULL);
    g_signal_connect (da,"configure-event",
                      G_CALLBACK (configure_event_cb), NULL);

    g_timeout_add(CALL_BACK_EVERY_MS, (GSourceFunc) time_handler, (gpointer) da);

    /* Event signals */
    g_signal_connect (da, "motion-notify-event",
                      G_CALLBACK (motion_notify_event_cb), NULL);
    g_signal_connect (da, "button-press-event",
                      G_CALLBACK (button_press_event_cb), NULL);

    /* Ask to receive events the drawing area doesn't normally
     * subscribe to. In particular, we need to ask for the
     * button press and motion notify events that want to handle.
     */
    gtk_widget_set_events (da, gtk_widget_get_events (da)
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_POINTER_MOTION_HINT_MASK);

    gtk_widget_show_all (window);

#ifndef GTK2
     GdkWindow *gdk_window = gtk_widget_get_window(window);

     printf("Pointer to gdkwindow [%x]\n", gdk_window);
     printf("Event compresssion: %d\n", gdk_window_get_event_compression (gdk_window));
//    gdk_window_set_event_compression (gtk_widget_get_parent_window (window), FALSE);
      gdk_window_set_event_compression (gdk_window, FALSE);
     printf("Event compresssion: %d\n", gdk_window_get_event_compression (gdk_window));
#endif


    gtk_main ();

    return 0;
}
