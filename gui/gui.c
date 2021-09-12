#include "gui.h"

static GtkViewport *viewport_composite;
static GtkImage *img_composite;
static GtkWindow *window_main;
static GtkWindow *window_about;
static GtkMenuItem *menu_save;
static GtkScrolledWindow *scrolled_composite;
GtkImage *_base_image = NULL;

float _sw, _sh;
int _zoom_changed = 0;

void
gui_init(GtkBuilder *builder)
{
	viewport_composite = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport_composite"));
	img_composite = GTK_IMAGE(gtk_builder_get_object(builder, "image_composite"));
	scrolled_composite = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledwindow_composite"));

	menu_save = GTK_MENU_ITEM(gtk_builder_get_object(builder, "menu_save"));

	window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
	window_about = GTK_WINDOW(gtk_builder_get_object(builder, "window_about"));
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(GTK_WIDGET(window_main));
}

void
on_window_main_destroy()
{
	if (_base_image) g_object_unref(_base_image);
	gtk_main_quit();
}

void
on_window_about_close()
{
	gtk_widget_hide(GTK_WIDGET(window_about));
}

void
show_about()
{
	gtk_widget_show(GTK_WIDGET(window_about));
}

void
set_save_sens(gboolean sens)
{
	gtk_widget_set_sensitive(GTK_WIDGET(menu_save), sens);
}

int
get_scrollable_image_width()
{
	return gtk_widget_get_allocated_width(GTK_WIDGET(viewport_composite));
}

int
get_scrollable_image_height()
{
	return gtk_widget_get_allocated_height(GTK_WIDGET(viewport_composite));
}

gboolean
on_composite_zoom_changed()
{
	GtkAdjustment *h_adj, *v_adj;
	float h_min, h_max, v_min, v_max;
	int window_w, window_h;

	/* If zoom didn't just change, don't scroll */
	if (!_zoom_changed) return FALSE;

	/* Get scrollbar adjustments */
	h_adj = gtk_scrolled_window_get_hadjustment(scrolled_composite);
	v_adj = gtk_scrolled_window_get_vadjustment(scrolled_composite);

	h_max = gtk_adjustment_get_upper(h_adj);
	h_min = gtk_adjustment_get_lower(h_adj);
	v_max = gtk_adjustment_get_upper(v_adj);
	v_min = gtk_adjustment_get_lower(v_adj);

	/* The adjustment value is the top of the scrollbar, but the clicked
	 * position should be in the center of the window, not at the top-left */
	window_w = get_scrollable_image_width();
	window_h = get_scrollable_image_height();

	/* Set scrollbar adjustments */
	gtk_adjustment_set_value(h_adj, h_min + (h_max - h_min) * _sw - window_w/2);
	gtk_adjustment_set_value(v_adj, v_min + (v_max - v_min) * _sh - window_h/2);
	_zoom_changed = 0;

	return FALSE;
}

void
preview_set_scroll(float scroll_w, float scroll_h)
{
	_sw = scroll_w;
	_sh = scroll_h;
	_zoom_changed = 1;
}

void
update_composite()
{
	g_idle_add((GSourceFunc)gtk_widget_queue_draw, (void*)img_composite);
}

void
update_title(char *title)
{
	gtk_window_set_title(window_main, title);
}
