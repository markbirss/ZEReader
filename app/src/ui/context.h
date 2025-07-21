/*
 * SPDX-FileCopyrightText: 2025 Anna-Lena Marx <mail@marx.engineer>
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

typedef enum
{
    MENU = 0,
    READING,
    SETTINGS

} context_t;

// When changing the enum, also change the corresponding strings
// in the .c file!
extern const char *context_strings[];

#endif