#include "gui.h"

static GtkImage *img_composite;
static GtkWindow *window_main;

void
gui_init(GtkBuilder *builder)
{
	img_composite = GTK_IMAGE(gtk_builder_get_object(builder, "image_composite"));

	window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(GTK_WIDGET(window_main));
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
