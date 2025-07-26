# SPDX-FileCopyrightText: 2025 Anna-Lena Marx <mail@marx.engineer>
#
# SPDX-License-Identifier: MPL-2.0

zephyr_library()

zephyr_library_include_directories(. ${ZEPHYR_BASE}/../modules/fs/fatfs/include)

zephyr_library_sources(
    sd.c
)
