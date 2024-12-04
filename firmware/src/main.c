/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <lvgl_input_device.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(main, CONFIG_DISPLAY_LOG_LEVEL);

static uint32_t count;

static void hello_world_button_clicked_cb(lv_event_t *e) {
	LOG_DBG("Hello World button clicked with event: %d", e->code);
}

int main(void)
{
	char count_str[15] = {0};
	LOG_DBG("Hello World - ZEReader! %s\n", CONFIG_BOARD_TARGET);

	const struct device *display_dev;
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

	//display_set_orientation(display_dev, DISPLAY_ORIENTATION_ROTATED_180);	
	
	lv_obj_t *hello_world_button;
	hello_world_button = lv_btn_create(lv_scr_act());
	lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_event_cb(hello_world_button, hello_world_button_clicked_cb, LV_EVENT_CLICKED, NULL);
	
	lv_obj_t *hello_world_label;
	hello_world_label = lv_label_create(hello_world_button);

	lv_label_set_text(hello_world_label, "My hello world button");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

	// lv_task_handler();

	// while (1) {
	// 	if ((count % 100) == 0U) {
	// 		sprintf(count_str, "Hello %d", count/100U);
	// 		lv_label_set_text(hello_world_label, count_str);
	// 	}
	// 	lv_task_handler();
	// 	++count;
	// 	k_sleep(K_MSEC(100));
	// }
}
