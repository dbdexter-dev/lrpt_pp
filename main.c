#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui/gui.h"

int
main(int argc, char *argv[])
{
	GtkBuilder *gtk_builder;

	gtk_init(&argc, &argv);
	gtk_builder = gtk_builder_new();
	gtk_builder_add_from_resource(gtk_builder, "/com/dbdexter/meteor_postprocess/glade/ui.glade", NULL);
	gui_init(gtk_builder);

	gtk_main();
}

void
on_window_main_destroy()
{
	gtk_main_quit();
}
