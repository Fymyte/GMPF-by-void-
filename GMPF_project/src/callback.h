//callback functions
void callback_image(GtkFileChooser *filebtn, gpointer user_data);
void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void callback_adjust_scale (GtkEntry *entry, gpointer user_data);
void callback_grey(GtkMenuItem *menuitem, gpointer user_data);
void callback_binarize(GtkMenuItem *menuitem, gpointer user_data);
void callback_FC(GtkMenuItem *menuitem, gpointer user_data);
void callback_hideWidget(GtkWidget *widget, gpointer user_data);
void callback_setCursor(GtkMenuItem *menuitem, gpointer user_data);

    // GMPF_LayerMngr
void callback_clear_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
void callback_add_GMPF_Layer(GtkMenuItem *menuitem, gpointer user_data);
void callback_remove_GMPF_LayerMngr(GtkMenuItem *menuitem, gpointer user_data);
