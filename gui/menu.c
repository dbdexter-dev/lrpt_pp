#include <gtk/gtk.h>
#include "effects/enhancements.h"

gboolean
on_radio_enha_normal_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	enhancement_set(NONE);
}

gboolean
on_radio_enha_122_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	enhancement_set(VISIBLE);
}

gboolean
on_radio_enha_vegetation_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	enhancement_set(VEGETATION);
}

gboolean
on_radio_enha_hvc_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	enhancement_set(HVC);
}

gboolean
on_radio_enha_thermal_toggled(GtkRadioButton *button)
{
	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(button))) return FALSE;
	enhancement_set(THERMAL);
}
