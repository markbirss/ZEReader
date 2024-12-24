cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ZEReader)

# zephyr_syscall_include_directories(include)
# zephyr_include_directories(include)

# # add_subdirectory(src/epub)
# add_subdirectory(lib)
#zephyr_include_directories(include/ui)

# das muss vermutlich beim Kompilieren von der UI schon verfuegbar sein?
add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

target_sources(app PRIVATE src/main.c)
