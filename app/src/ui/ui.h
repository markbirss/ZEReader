#ifndef _UI_H_
#define _UI_H_

#include <lvgl.h>
#include <lvgl_input_device.h>

#include "context.h"

extern lv_obj_t *button_left;
extern lv_obj_t *button_mid;
extern lv_obj_t *button_right;

extern lv_obj_t *button_left_label;
extern lv_obj_t *button_mid_label;
extern lv_obj_t *button_right_label;

void zereader_setup_control_buttons(context_t *context);
void zereader_setup_page();

void zereader_print_page();
void zereader_clean_page();

void zereader_show_logo();
void zereader_clean_logo();
#endif