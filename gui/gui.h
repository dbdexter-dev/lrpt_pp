#ifndef gui_h
#define gui_h

#include <gtk/gtk.h>

void gui_init(GtkBuilder *builder);
void update_composite();
void update_title(char *title);

GtkWindow* gui_main_window();

int get_scrollable_image_width();
int get_scrollable_image_height();

void set_save_sens(gboolean sens);

#endif
