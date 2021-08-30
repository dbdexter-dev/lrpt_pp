#include <gtk/gtk.h>
#include <string.h>
#include "composite.h"
#include "gui.h"
#include "save.h"

char*
save_composite(char *fname)
{
	const char *default_ext = "png";
	char *extptr;
	GError *error = NULL;
	uint8_t *pixels;
	GdkPixbuf *pixbuf;
	GtkWidget *err_dialog;

	/* Infer the file type from extension */
	extptr = fname + strlen(fname);
	for (extptr--; *extptr != '.' && extptr > fname; extptr--) ;
	if (extptr == fname) extptr = default_ext;
	else extptr++;

	/* Wrap composite in a GdkPixbuf container */
	pixels = composite_get_pixels();
	pixbuf = gdk_pixbuf_new_from_data(
			pixels,
			GDK_COLORSPACE_RGB,
			0,
			8,
			composite_get_width(),
			composite_get_height(),
			composite_get_rowstride(),
			NULL,
			NULL);

	/* Save to file */
	if (gdk_pixbuf_save(pixbuf, fname, extptr, &error, NULL) == FALSE) {
		composite_release_pixels();
		return error->message;
	}
	printf("Saved %s to %s\n", extptr, fname);


	composite_release_pixels();
	return NULL;
}
