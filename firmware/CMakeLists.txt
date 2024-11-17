cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ZEReader)

# add_subdirectory(src/epub)
# add_subdirectory(src/libzip)

target_sources(app PRIVATE src/main.c)
