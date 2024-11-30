/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <lvgl.h>
#include <lvgl_input_device.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

// struct input_event last_input_event;

static void hello_world_button_clicked_cb(lv_event_t *e) {
	LOG_INF("Hello World button clicked with event: %d", e->code);
}

int main(void)
{
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	const struct device *display_dev;
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

	// lv_obj_t *arc;
	// lv_group_t *arc_group;

	// arc = lv_arc_create(lv_scr_act());
	// lv_obj_align(arc, LV_ALIGN_CENTER, 0, -15);
	// lv_obj_set_size(arc, 150, 150);

	// arc_group = lv_group_create();
	// lv_group_add_obj(arc_group, arc);
	
	lv_obj_t *hello_world_button;
	hello_world_button = lv_btn_create(lv_scr_act());
	lv_obj_align(hello_world_button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	lv_obj_add_event_cb(hello_world_button, hello_world_button_clicked_cb, LV_EVENT_CLICKED, NULL);
	
	lv_obj_t *hello_world_label;
	hello_world_label = lv_label_create(hello_world_button);

	lv_label_set_text(hello_world_label, "My hello world button");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);


	return 0;
}
