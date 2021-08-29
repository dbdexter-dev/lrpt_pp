#ifndef gui_h
#define gui_h

#include <gtk/gtk.h>

void gui_init(GtkBuilder *builder);
void update_composite();
void update_title(char *title);

#endif
