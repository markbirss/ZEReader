#include <zephyr/logging/log.h>

#include "ui.h"
#include "../epub/epub.h"

LOG_MODULE_REGISTER(ui, CONFIG_ZEREADER_LOG_LEVEL);

lv_obj_t *button_4;
lv_obj_t *button_3;
lv_obj_t *button_2;
lv_obj_t *button_1;

lv_obj_t *button_4_label;
lv_obj_t *button_3_label;
lv_obj_t *button_2_label;
lv_obj_t *button_1_label;

lv_obj_t *text_area;
lv_obj_t *logo;

void zereader_print_prev_page();
void zereader_print_next_page();

static void button_1_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Button 1 clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	LOG_DBG("Event code: %d", lv_event_get_code(e));
	zereader_print_prev_page();
}

static void button_2_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Button 2 clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	zereader_clean_page();
	zereader_clean_logo();
}

static void button_3_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Button 3 clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	zereader_show_logo();
}

static void button_4_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Button 4 clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	zereader_print_next_page();
}

void zereader_setup_control_buttons(context_t *context)
{
	lv_obj_set_style_text_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);

	LOG_DBG("Button Setup");
	button_1 = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_left, &style_default, 0);
	LOG_DBG("Button created");
	lv_obj_align(button_1, LV_ALIGN_BOTTOM_LEFT, 10, -5);
	LOG_DBG("Button aligned");
	lv_obj_add_event_cb(button_1, button_1_clicked_cb, LV_EVENT_CLICKED, context);
	LOG_DBG("Button callback added");

	button_1_label = lv_label_create(button_1);
	lv_label_set_text(button_1_label, "prev page");
	lv_obj_center(button_1_label);

	button_2 = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_mid, &style_default, 0);
	lv_obj_align(button_2, LV_ALIGN_BOTTOM_MID, -50, -5);
	lv_obj_add_event_cb(button_2, button_2_clicked_cb, LV_EVENT_CLICKED, context);

	button_2_label = lv_label_create(button_2);
	lv_label_set_text(button_2_label, "menu");
	lv_obj_center(button_2_label);

	button_3 = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_right, &style_default, 0);
	lv_obj_align(button_3, LV_ALIGN_BOTTOM_MID, 50, -5);
	lv_obj_add_event_cb(button_3, button_3_clicked_cb, LV_EVENT_CLICKED, context);

	button_3_label = lv_label_create(button_3);
	lv_label_set_text(button_3_label, "settings");
	lv_obj_center(button_3_label);

	button_4 = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_right, &style_default, 0);
	lv_obj_align(button_4, LV_ALIGN_BOTTOM_RIGHT, -10, -5);
	lv_obj_add_event_cb(button_4, button_4_clicked_cb, LV_EVENT_CLICKED, context);

	button_4_label = lv_label_create(button_4);
	lv_label_set_text(button_4_label, "next page");
	lv_obj_center(button_4_label);
}

void zereader_setup_page()
{
	LOG_DBG("Setup page");
	text_area = lv_label_create(lv_screen_active());
	LOG_DBG("Created page label");
	// lv_obj_align(text_area, LV_ALIGN_TOP_LEFT, 10, 20);
	// LOG_DBG("Aligned page label");
	// lv_textarea_set_max_length(text_area, 400);
	lv_obj_set_x(text_area, 10);
	lv_obj_set_y(text_area, 20);
	lv_obj_set_width(text_area, 750);
	lv_obj_set_height(text_area, 440);
	// LOG_DBG("Text area length: %d", lv_textarea_get_max_length(text_area));
}

void zereader_print_next_page()
{
	lv_label_set_text(text_area, epub_get_next_page());
}

void zereader_print_prev_page()
{
	lv_label_set_text(text_area, epub_get_prev_page());
}

void zereader_clean_page()
{
	lv_obj_invalidate(lv_screen_active());
	lv_label_set_text(text_area, " ");
}

void zereader_show_logo()
{
	LV_IMG_DECLARE(zereaderlogomarx);
	logo = lv_image_create(lv_screen_active());
	lv_image_set_src(logo, &zereaderlogomarx);
	lv_obj_align(logo, LV_ALIGN_CENTER, 0, 30);
}

void zereader_clean_logo()
{
	if (logo != NULL)
	{
		lv_obj_del(logo);
		logo = NULL;
	}
}