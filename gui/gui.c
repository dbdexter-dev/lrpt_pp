#include "gui.h"

static GtkViewport *img_viewport;
static GtkImage *img_composite;
static GtkWindow *window_main;
static GtkMenuItem *menu_save;

void
gui_init(GtkBuilder *builder)
{
	img_viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport_composite"));
	img_composite = GTK_IMAGE(gtk_builder_get_object(builder, "image_composite"));
	menu_save = GTK_MENU_ITEM(gtk_builder_get_object(builder, "menu_save"));

	window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(GTK_WIDGET(window_main));
}

GtkWindow*
gui_main_window()
{
	return window_main;
}

void
set_save_sens(gboolean sens)
{
	gtk_widget_set_sensitive(GTK_WIDGET(menu_save), sens);
}

int
get_scrollable_image_width()
{
	return gtk_widget_get_allocated_width(GTK_WIDGET(img_viewport));
}

int
get_scrollable_image_height()
{
	return gtk_widget_get_allocated_height(GTK_WIDGET(img_viewport));
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
