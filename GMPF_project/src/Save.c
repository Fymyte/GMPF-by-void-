#include "Save.h"

int export_cairo_to_png(SGlobalData *data)
{
    cairo_surface_t *final_surface = NULL;
    cairo_t *final_context = cairo_create(final_surface);

    GMPF_LayerMngr *layermngr = NULL;
    GtkFlowBox *flowbox = NULL;

    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    if (layermngr->layer_list.next == NULL)
    {
        printf("toz\n");
        return -1; //save failed
    }
    cairo_arc(final_context, 0, 0, 10, 0.0, G_PI * 2);
    cairo_fill_preserve(final_context);
    cairo_paint(final_context);

    /*GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);

    while (lay != NULL)
    {
        if (lay->isvisible)
        {
            cairo_save(final_context);
            // cairo_surface_t *surface = lay->surface ? lay->surface : lay->surface;
            cairo_scale(final_context, lay->scale_factor.x, lay->scale_factor.y);
            cairo_set_source_surface (final_context, lay->surface, (double)lay->pos.x, (double)lay->pos.y);
            cairo_paint(final_context);
            cairo_restore(final_context);
        }
        if (!lay->list.next) break;
        lay = container_of(lay->list.next, GMPF_Layer, list);
    }

    */
    //char filename = malloc();
    final_surface = cairo_get_target(final_context);
    cairo_status_t status = cairo_surface_write_to_png (final_surface, "image.png");
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

    else if (status == CAIRO_STATUS_PNG_ERROR)
    {
        printf("%s\n", cairo_status_to_string(status));
        return -1;
    }

    //if (status == CAIRO_STATUS_SUCCESS)
    printf("fail\n");
    return 0;
}
