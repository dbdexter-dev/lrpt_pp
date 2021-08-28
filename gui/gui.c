#include "gui.h"

static GtkImage *img_composite;

void
gui_init(GtkBuilder *builder)
{
	GtkWidget *window;

	img_composite = GTK_IMAGE(gtk_builder_get_object(builder, "image_composite"));

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(window);
}

void
update_composite()
{
	g_idle_add((GSourceFunc)gtk_widget_queue_draw, (void*)img_composite);
}
