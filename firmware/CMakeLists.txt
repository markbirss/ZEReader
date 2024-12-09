cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ZEReader)

# add_subdirectory(src/epub)
add_subdirectory(src/ui)

add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

target_sources(app PRIVATE src/main.c)
