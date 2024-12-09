#include <zephyr/logging/log.h>

#include "ui.h"

LOG_MODULE_REGISTER(ui);


static void button_left_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Left button clicked with event: %d", e->code);
    LOG_DBG("Context: %d", (int) e->user_data);
}

static void button_mid_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Middle button clicked with event: %d", e->code);
	LOG_DBG("Context: %d", (int) e->user_data);
}

static void button_right_clicked_cb(lv_event_t *e)
{
	LOG_DBG("Right button clicked with event: %d", e->code);
	LOG_DBG("Context: %d", (int) e->user_data);
}


void zereader_setup_contol_buttons(context_t *context) {
	button_left = lv_btn_create(lv_scr_act());
	lv_obj_align(button_left, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_add_event_cb(button_left, button_left_clicked_cb, LV_EVENT_CLICKED, context);

	button_left_label = lv_label_create(button_left);
	lv_label_set_text(button_left_label, "Left Button");
	lv_obj_center(button_left_label);


	button_mid = lv_btn_create(lv_scr_act());
	lv_obj_align(button_mid, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_add_event_cb(button_mid, button_mid_clicked_cb, LV_EVENT_CLICKED, context);

	button_mid_label = lv_label_create(button_mid);
	lv_label_set_text(button_mid_label, "Mid Button");
	lv_obj_center(button_mid_label);


	button_right = lv_btn_create(lv_scr_act());
	lv_obj_align(button_right, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
	lv_obj_add_event_cb(button_right, button_right_clicked_cb, LV_EVENT_CLICKED, context);

	button_right_label = lv_label_create(button_right);
	lv_label_set_text(button_right_label, "Right Button");
	lv_obj_center(button_right_label);
}