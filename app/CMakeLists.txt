cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ZEReader)

zephyr_library_include_directories(src/epub)
zephyr_library_include_directories(src/ui)

add_subdirectory(src/epub)
add_subdirectory(src/ui)

target_sources(app PRIVATE src/main.c)
