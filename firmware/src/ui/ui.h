#pragma once

#include <lvgl.h>
#include <lvgl_input_device.h>

#include "context.h"

lv_obj_t *button_left;
lv_obj_t *button_mid;
lv_obj_t *button_right;

lv_obj_t *button_left_label;
lv_obj_t *button_mid_label;
lv_obj_t *button_right_label;


void zereader_setup_contol_buttons(context_t *context);