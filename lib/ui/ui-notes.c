// #include <lvgl.h>
// #include <lvgl_input_device.h>
// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/display.h>
// #include <zephyr/kernel.h>

// static uint8_t buffer[(HORIZONTAL_RES * VERTICAL_RES / 8) + 8];

// lv_display_t *display = lv_display_create(HORIZONTAL_RES, VERTICAL_RES);

// lv_display_set_buffers(display, buffer, NULL, sizeof(buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

// // Set monochrome display colorformat for epaper
// lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);

// // More on monochrome
// https://docs.lvgl.io/master/details/main-components/display.html#monochrome-displays

// Display is configured via device tree

// https://docs.lvgl.io/master/details/main-components/indev.html#keypad-or-keyboard
// https://docs.lvgl.io/master/details/main-components/indev.html#keypad-or-keyboard

// https://docs.zephyrproject.org/latest/build/dts/api/bindings/input/zephyr,lvgl-keypad-input.html
// https://docs.zephyrproject.org/latest/build/dts/api/bindings/input/zephyr%2Clvgl-button-input.html#std-dtcompatible-zephyr-lvgl-button-input

// https://docs.lvgl.io/master/details/widgets/win.html


    // buttons: buttons {
    //     compatible = "gpio-keys";
    //     button1: button_1 {
    //         label = "top-right";
    //         gpios = <&gpio0 27 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
    //         zephyr,code = <INPUT_KEY_1>;
    //     };
    //     button2: button_2 {
    //         label = "bottom-left";
    //         gpios = <&gpio0 30 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
    //         zephyr,code = <INPUT_KEY_2>;
    //     };
    //     button3: button_3 {
    //         label = "bottom-right";
    //         gpios = <&gpio1 12 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
    //         zephyr,code = <INPUT_KEY_KP0>;
    //     };
    //     button4: button_4 {
    //         label = "top-left";
    //         gpios = <&gpio1 6 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
    //         zephyr,code = <INPUT_KEY_4>;
    //     };
    // };

    // pointer {
    //     compatible = "zephyr,lvgl-button-input";
    //     input = <&buttons>;
    //     input-codes = <INPUT_KEY_0 INPUT_KEY_1>;
    //     coordinates = <120 220>, <150 250>;
    // };

// int setup_display() {
//     const struct device *display_dev;
//     display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
// 	if (!device_is_ready(display_dev)) {
// 		LOG_ERR("Device not ready, aborting test");
// 		return 0;
// 	}


// }

// int render_book_page() {

// }

// int render_library_list() {
    
// }