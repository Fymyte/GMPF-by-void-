#include "Save.h"

int export_cairo_to_png(SGlobalData *data)
{
    GMPF_LayerMngr *layermngr = NULL;
    GtkFlowBox *flowbox = NULL;

    flowbox = (GtkFlowBox *) (gtk_builder_get_object(data->builder, "GMPF_flowbox"));
    layermngr = layermngr_get_layermngr(flowbox);

    if (layermngr->layer_list.next == NULL)
        return -1; //save failed
    
    

	GMPF_Layer *lay = container_of(layermngr->layer_list.next, GMPF_Layer, list);

	//create a new surface and context to store all the layer surfaces
	cairo_surface_t *final_surface =
		cairo_image_surface_create (CAIRO_FORMAT_ARGB32, lay->size.w, lay->size.h);
	cairo_t *final_context = cairo_create(final_surface);

	//write the surfaces on the new surface
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

   
	//get the surface from the context and save it
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

    //save succeed
    return 0;
}
