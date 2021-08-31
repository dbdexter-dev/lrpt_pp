#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "composite.h"
#include "effects/enhancements.h"
#include "utils.h"
#ifdef ENABLE_GTK
#include <gtk/gtk.h>
#include "gui/gui.h"
#endif

#define SHORTOPTS "e:hv"

static struct option longopts[] = {
	{ "enhance",      1, NULL, 'e' },
	{ "help",         0, NULL, 'h' },
	{ "version",      0, NULL, 'v' },
};

int
main(int argc, char *argv[])
{
	Enhancement enha;
	int c;
#ifdef ENABLE_GTK
	GtkBuilder *gtk_builder;
#endif

	/* Command-line changeable parameters {{{ */
	int start_gui = 1;
	const char *enha_str = NULL;
	/* }}} */
	/* Parse command-line options {{{ */
	while ((c = getopt_long(argc, argv, SHORTOPTS, longopts, NULL)) != -1) {
		switch (c) {
			case 'e':
				start_gui = 0;
				enha_str = optarg;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			case 'v':
				version();
				return 0;
		}
	}
	/* }}} */

#ifdef ENABLE_GTK
	if (start_gui) {
		gtk_init(&argc, &argv);
		gtk_builder = gtk_builder_new();
		gtk_builder_add_from_resource(gtk_builder, "/com/dbdexter/meteor_postprocess/glade/ui.glade", NULL);
		gui_init(gtk_builder);

		gtk_main();
		return 0;
	} else {
#endif
	if (!enha_str) {
		fprintf(stderr, "Error: no enhancement specified\n");
		return 1;
	}

	/* TODO if no input file, error and exit */
	/* TODO if no output file, generate a filename */
	/* TODO open input file, create a Image obj */

	if (!strcmp(enha_str, "none")) {
		enha = NONE;
	} else if (!strcmp(enha_str, "rgb122")) {
		enha = VISIBLE;
	} else if (!strcmp(enha_str, "rgb211")) {
		enha = VEG_IR;
	} else if (!strcmp(enha_str, "vegetation")) {
		enha = VEGETATION;
	} else if (!strcmp(enha_str, "thermal")) {
		enha = THERMAL;
	} else if (!strcmp(enha_str, "hvc")) {
		enha = HVC;
	} else if (!strcmp(enha_str, "hvc_precip")) {
		enha = HVC_PRECIP;
	} else {
		fprintf(stderr, "Error: unknown enhancement '%s'\n", enha_str);
		return 1;
	}

	/* TODO apply enhancement to Image */
	/* TODO write Image to file */


#ifdef ENABLE_GTK
	}   /* if (start_gui) {} else {*/
#endif

	return 0;
}

