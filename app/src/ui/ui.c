#include <zephyr/logging/log.h>

#include "ui.h"
#include "../epub/epub.h"

LOG_MODULE_REGISTER(ui, CONFIG_ZEREADER_LOG_LEVEL);

lv_obj_t *button_left;
lv_obj_t *button_mid;
lv_obj_t *button_right;

lv_obj_t *button_left_label;
lv_obj_t *button_mid_label;
lv_obj_t *button_right_label;

lv_obj_t *text_area;

void zereader_print_prev_page();
void zereader_print_next_page();

static void button_left_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Left button clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	LOG_DBG("Event code: %d", lv_event_get_code(e));
	zereader_print_prev_page();
}

static void button_mid_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Middle button clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	zereader_clean_page();
}

static void button_right_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Right button clicked with event");
	context_t *context = lv_event_get_user_data(e);
	LOG_DBG("Context: %s", context_strings[*context]);
	zereader_print_next_page();
}

void zereader_setup_control_buttons(context_t *context)
{
	lv_obj_set_style_text_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);

	LOG_DBG("Button Setup");
	button_left = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_left, &style_default, 0);
	LOG_DBG("Button created");
	lv_obj_align(button_left, LV_ALIGN_BOTTOM_LEFT, 10, -5);
	LOG_DBG("Button aligned");
	lv_obj_add_event_cb(button_left, button_left_clicked_cb, LV_EVENT_CLICKED, context);
	LOG_DBG("Button callback added");

	button_left_label = lv_label_create(button_left);
	lv_label_set_text(button_left_label, "prev page");
	lv_obj_center(button_left_label);

	button_mid = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_mid, &style_default, 0);
	lv_obj_align(button_mid, LV_ALIGN_BOTTOM_MID, 0, -5);
	lv_obj_add_event_cb(button_mid, button_mid_clicked_cb, LV_EVENT_CLICKED, context);

	button_mid_label = lv_label_create(button_mid);
	lv_label_set_text(button_mid_label, "menu");
	lv_obj_center(button_mid_label);

	button_right = lv_button_create(lv_screen_active());
	// lv_obj_add_style(button_right, &style_default, 0);
	lv_obj_align(button_right, LV_ALIGN_BOTTOM_RIGHT, -10, -5);
	lv_obj_add_event_cb(button_right, button_right_clicked_cb, LV_EVENT_CLICKED, context);

	button_right_label = lv_label_create(button_right);
	lv_label_set_text(button_right_label, "next page");
	lv_obj_center(button_right_label);
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