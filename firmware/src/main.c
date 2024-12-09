
#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>

#include "ui/ui.h"

LOG_MODULE_REGISTER(main, CONFIG_DISPLAY_LOG_LEVEL);


/*
This can be used to access the GPIO Buttons directly.
If used together with the LVGL input device, there is some
concurrency who handles the event.
So better just use one mechanism!
*/

// static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(
// 	DT_ALIAS(sw0), gpios, {0});
// static struct gpio_callback button_callback;

// static void button_isr_callback(const struct device *port,
// 								struct gpio_callback *cb,
// 								uint32_t pins)
// {
// 	ARG_UNUSED(port);
// 	ARG_UNUSED(cb);
// 	ARG_UNUSED(pins);

// 	LOG_DBG("GPIO button clicked");
// 	// count = 0;
// }



int main(void)
{
	LOG_DBG("Hello World - ZEReader! %s\n", CONFIG_BOARD_TARGET);

	context_t context = BOOK_MENU;

	// Initialize the choosen zephyr,display device
	// -> Make the device tree description available for the software part
	const struct device *display_dev;
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device not ready, aborting...");
		return 0;
	}

	// Make the FIRST ok zephyr,lvgl-button-input node available to the software part
	static const struct device *lvgl_btn_dev;
	lvgl_btn_dev = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_button_input));
	if (!device_is_ready(lvgl_btn_dev))
	{
		LOG_ERR("Device not ready, aborting...");
		return 0;
	}

	/*
	Initializing the GPIOs by hand is not needed when using the lvgl-button-input driver
	*/
	// if (gpio_is_ready_dt(&button_gpio))
	// {
	// 	int err;

	// 	err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
	// 	if (err)
	// 	{
	// 		LOG_ERR("failed to configure button gpio: %d", err);
	// 		return 0;
	// 	}

	// 	gpio_init_callback(&button_callback, button_isr_callback,
	// 					   BIT(button_gpio.pin));

	// 	err = gpio_add_callback(button_gpio.port, &button_callback);
	// 	if (err)
	// 	{
	// 		LOG_ERR("failed to add button callback: %d", err);
	// 		return 0;
	// 	}

	// 	err = gpio_pin_interrupt_configure_dt(&button_gpio,
	// 										  GPIO_INT_EDGE_TO_ACTIVE);
	// 	if (err)
	// 	{
	// 		LOG_ERR("failed to enable button callback: %d", err);
	// 		return 0;
	// 	}
	// }

	// lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_180);

	zereader_setup_contol_buttons(&context);

	lv_task_handler();
	display_blanking_off(display_dev);

	lv_task_handler();

	while (1) {
		// if ((count % 100) == 0U) {
		// 	sprintf(count_str, "Hello %d", count/100U);
		// 	lv_label_set_text(hello_world_label, count_str);
		// }
		lv_task_handler();
		// ++count;
		k_sleep(K_MSEC(10));
	}
}
