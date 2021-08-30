#include <gtk/gtk.h>
#include "composite.h"
#include "gui.h"
#include "utils.h"

static int zoom = 0;

gboolean
on_image_composite_draw(GtkWidget *widget, cairo_t *cr)
{
	float scale;
	int w_orig, h_orig, w_draw, h_draw, w_new, h_new;

	GdkPixbuf *composite, *scaled;
	uint8_t *pixels;

	pixels = composite_get_pixels();
	if (!pixels) {
		composite_release_pixels();
		return FALSE;
	}

	/* Fetch the current composite pixbuf */
	composite = gdk_pixbuf_new_from_data(
			pixels,
			GDK_COLORSPACE_RGB,
			0,
			8,
			composite_get_width(),
			composite_get_height(),
			composite_get_rowstride(),
			NULL,
			NULL);

	w_orig = gdk_pixbuf_get_width(composite);
	h_orig = gdk_pixbuf_get_height(composite);
	w_draw = get_scrollable_image_width();
	h_draw = get_scrollable_image_height();

	if (zoom) {
		w_new = w_orig;
		h_new = h_orig;
	} else {
		/* Compute the max composite size that can fit while keeping the aspect ratio */
		scale = MIN(w_draw/(float)w_orig, h_draw/(float)h_orig);
		w_new = MIN(w_draw-1, w_orig*scale);
		h_new = MIN(h_draw-1, h_orig*scale);
	}

	gtk_widget_set_size_request(widget, w_new, h_new);

	/* Scale the image and paint it */
	scaled = gdk_pixbuf_scale_simple(composite, w_new, h_new, GDK_INTERP_BILINEAR);
	gdk_cairo_set_source_pixbuf(cr, scaled, 0, 0);
	cairo_paint(cr);

	/* Cleanup */
	g_object_unref(scaled);
	g_object_unref(composite);
	composite_release_pixels();
	return FALSE;
}

gboolean
on_image_composite_button_release_event(GtkWidget *widget, GdkEvent *e, gpointer p)
{
	zoom = !zoom;
	update_composite();
}
