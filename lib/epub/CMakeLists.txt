zephyr_library()

zephyr_library_include_directories(${ZEPHYR_BASE}/../modules/fs/fatfs/include)

zephyr_library_sources(
    sd_card.c
)