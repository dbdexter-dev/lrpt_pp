#ifndef gui_h
#define gui_h

#include <gtk/gtk.h>

extern GtkImage *_base_image;

void gui_init(GtkBuilder *builder);
void update_composite();
void update_title(char *title);
void update_worker_status(char *msg, int active);

int get_scrollable_image_width();
int get_scrollable_image_height();
void preview_set_scroll(float scroll_w, float scroll_h);

void set_save_sens(gboolean sens);

void show_about();

#endif
