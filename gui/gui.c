#include "gui.h"

static GtkImage *img_composite;
static GtkViewport *img_viewport;
static GtkScrolledWindow *img_scrolledwindow;
static GtkEventBox *img_eventbox;
static GtkWindow *window_main;

void
gui_init(GtkBuilder *builder)
{
	img_composite = GTK_IMAGE(gtk_builder_get_object(builder, "image_composite"));
	img_viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport_composite"));
	img_scrolledwindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledwindow_composite"));
	img_eventbox = GTK_EVENT_BOX(gtk_builder_get_object(builder, "eventbox_composite"));

	window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(GTK_WIDGET(window_main));
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
