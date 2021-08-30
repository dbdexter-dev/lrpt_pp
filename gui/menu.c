#include <gtk/gtk.h>
#include "composite.h"
#include "gui.h"

static GtkImage *_composite;
static Enhancement _last_enhancement = NONE;

extern void on_window_main_destroy();

gboolean
on_radio_enha_normal_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(NONE, update_composite);
	_last_enhancement = NONE;
}

gboolean
on_radio_enha_122_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(VISIBLE, update_composite);
	_last_enhancement = VISIBLE;
}

gboolean
on_radio_enha_vegetation_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(VEGETATION, update_composite);
	_last_enhancement = VEGETATION;
}

gboolean
on_radio_enha_211_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(VEG_IR, update_composite);
	_last_enhancement = VEG_IR;
}

gboolean
on_radio_enha_hvc_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(HVC, update_composite);
	_last_enhancement = HVC;
}

gboolean
on_radio_enha_hvc_precip_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(HVC_PRECIP, update_composite);
	_last_enhancement = HVC_PRECIP;
}

gboolean
on_radio_enha_thermal_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	composite_set_enhancement(THERMAL, update_composite);
	_last_enhancement = THERMAL;
}

gboolean
on_menu_quit_activate()
{
	on_window_main_destroy();
	return FALSE;
}

gboolean
on_menu_open_activate()
{
	GtkWidget *dialog;
	GdkPixbuf *pixbuf;
	int width, height, rowstride;
	int result;
	char *fname;

	dialog = gtk_file_chooser_dialog_new(
			"Open file",
			NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"_Cancel",
			GTK_RESPONSE_CANCEL,
			"_Open",
			GTK_RESPONSE_ACCEPT,
			NULL);

	result = gtk_dialog_run(GTK_DIALOG(dialog));

	switch (result) {
		case GTK_RESPONSE_ACCEPT:
			if (_composite) {
				g_object_ref_sink(_composite);
				_composite = NULL;
			}

			/* Load composite from file */
			fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			_composite = GTK_IMAGE(gtk_image_new_from_file(fname));
			pixbuf = gtk_image_get_pixbuf(_composite);

			/* Update composite channels */
			width = gdk_pixbuf_get_width(pixbuf);
			height = gdk_pixbuf_get_height(pixbuf);
			rowstride = gdk_pixbuf_get_rowstride(pixbuf);
			composite_init(gdk_pixbuf_get_pixels(pixbuf), width, height, rowstride);
			composite_set_enhancement(_last_enhancement, update_composite);

			update_title(fname);
			break;
		case GTK_RESPONSE_CANCEL:
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);
	return TRUE;
}
