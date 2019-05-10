#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H


#include <gtk/gtk.h>
#include <stdlib.h>
#include <err.h> // for the errors
#include <stdio.h> // for the errors

#include "types_redefine.h"
#include "GMPF_Struct.h"
#include "buffer.h"
#include "list.h"
#include "saving.h"
#include "callback.h"

#include "macro.h"

// DEFINES
#define LAYERMNGR_KEY_NAME "GMPF_layermngr"
#define LAYER_KEY_NAME "GMPF_layer"
#define SAVED_STATE_KEY_NAME "GMPF_saved_state"
#define SELECTION_KEY_NAME "GMPF_selection"
#define BUFFER_KEY_NAME "GMPF_buffer"

#define INIT_LAYER_UI(_layer, _image, _grid) \
    GtkWidget *_grid = gtk_grid_new(); \
    GtkWidget *gtklabel = gtk_label_new(_layer->name); \
    GtkWidget *button = gtk_check_button_new(); \
    gtk_toggle_button_set_active((GtkToggleButton *)button, _layer->isvisible); \
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(callback_layer_set_visible), NULL); \
    gtk_widget_set_sensitive(_grid, TRUE); \
    gtk_widget_set_visible(_grid, TRUE); \
    gtk_widget_set_visible(_image, TRUE); \
    gtk_widget_set_visible(gtklabel, TRUE); \
    gtk_widget_set_visible(button, TRUE); \
    /*** Visual ***/ \
    /* size */ \
    gtk_widget_set_size_request(_image, 160, 90); \
    gtk_widget_set_size_request(gtklabel, 130, 30); \
    gtk_widget_set_size_request(_grid, 160, 120); \
    gtk_widget_set_size_request(button, 20, 20); \
    /* Alignement */ \
    gtk_label_set_justify((GtkLabel *)gtklabel, GTK_JUSTIFY_RIGHT); \
    gtk_widget_set_halign(_grid, GTK_ALIGN_CENTER); \
    gtk_widget_set_valign(_grid, GTK_ALIGN_START); \
    /* Margin */ \
    gtk_widget_set_margin_top(_grid, 5); \
    gtk_widget_set_margin_bottom(_grid, 5); \
    gtk_widget_set_margin_start(_grid, 5); \
    gtk_widget_set_margin_end(_grid, 5); \
    gtk_widget_set_margin_top(button, 5); \
    gtk_widget_set_margin_bottom(button, 5); \
    gtk_widget_set_margin_start(button, 5); \
    gtk_widget_set_margin_end(button, 5); \
    gtk_grid_attach((GtkGrid *)_grid, _image, 0, 0, 2, 1); \
    gtk_grid_attach((GtkGrid *)_grid, gtklabel, 1, 1, 1, 1); \
    gtk_grid_attach((GtkGrid *)_grid, button, 0, 1, 1, 1);

/*****************************Selection functions******************************/

GMPF_Selection *GMPF_selection_init(GtkFlowBox *flowbox);
void GMPF_selection_destroy(GtkFlowBox *flowbox);
GMPF_Selection *GMPF_selection_get_selection(GtkFlowBox *flowbox);
GMPF_Selection *GMPF_selection_set_selection(GtkFlowBox     *flowbox,
                                             GMPF_Selection *selection);
cairo_surface_t *GMPF_selection_get_surface(GtkFlowBox *flowbox);
int GMPF_selection_set_surface(GtkFlowBox      *flowbox,
                               cairo_surface_t *surface);
GMPF_Pos *GMPF_selection_get_pos(GtkFlowBox *flowbox);
int GMPF_selection_set_pos(GtkFlowBox *flowbox,
                           GMPF_Pos    pos);
GMPF_Size *GMPF_selection_get_size(GtkFlowBox *flowbox);
int GMPF_selection_set_size(GtkFlowBox *flowbox,
                           GMPF_Size    size);
char GMPF_selection_copy(GtkFlowBox *flowbox,
                         GMPF_Layer *layer);
char GMPF_selection_paste(GtkFlowBox *flowbox,
                          GMPF_Layer *layer,
                          GMPF_Pos    pos);
char GMPF_selection_cut(GtkFlowBox *flowbox,
                        GMPF_Layer *layer);
char GMPF_selection_delete(GtkFlowBox *flowbox,
                           GMPF_Layer *layer);

/*************************End of Selection functions***************************/

/****************************Saved State functions*****************************/

GMPF_SavedState *GMPF_saved_state_init(GtkFlowBox *flowbox);
void GMPF_saved_state_destroy(GtkFlowBox *flowbox);
GMPF_SavedState *GMPF_saved_state_get_saved_state(GtkFlowBox *flowbox);
int GMPF_saved_state_get_is_saved(GtkFlowBox *flowbox);
void GMPF_saved_state_set_is_saved(GtkFlowBox *flowbox, int state);

/*************************End of Saved State functions*************************/

/******************************LayerMngr functions*****************************/

void layermngr_create(GtkFlowBox *flowbox); //ENDED
void layermngr_set_to_flowbox(GtkFlowBox *flowbox, GMPF_LayerMngr *layermngr);
void layermngr_initialization(GMPF_LayerMngr *layermngr); //ENDED
void layermngr_clear(GtkFlowBox *flowbox); //ENDED
void layermngr_delete(GtkFlowBox *flowbox); //ENDED

void layermngr_move_down_selected_layer(GtkFlowBox *flowbox);
void layermngr_move_up_selected_layer(GtkFlowBox *flowbox);

GMPF_Layer * layermngr_get_selected_layer(GtkFlowBox *flowbox); //ENDED
GMPF_Layer *layermngr_add_new_layer(GtkFlowBox *flowbox, const char *filename); //ENDED
void layermngr_delete_selected_layer(GtkFlowBox *flowbox); //ENDED

GMPF_LayerMngr *layermngr_get_layermngr(GtkFlowBox *flowbox);

/**************************End of LayerMngr functions**************************/

/********************************Layer functions*******************************/

GMPF_Layer * layer_initialization(); //ENDED
void layer_delete(GMPF_Layer *layer); //ENDED
void layer_insert_at_pos(GMPF_Layer *layer,
                         GtkFlowBox *flowbox,
                         int         insertpos);
GMPF_Layer *layer_get_at_pos(GtkFlowBox *flowbox,
                             int         pos);
char layer_set_name(GMPF_Layer *layer,
                    char       *name);
void layer_icon_refresh(GMPF_Layer *layer);

/****************************End of Layer functions****************************/

GdkPixbuf * new_pixbuf_standardized(GMPF_Size *size);
int pixbuf_standardized(GdkPixbuf **pixbuf);

#endif /* LAYER_MANAGER_H */
