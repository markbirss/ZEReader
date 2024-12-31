#pragma once

typedef enum
{
    MENU = 0,
    READING,
    SETTINGS

} context_t;

// When changing the enum, also change the corresponding strings
// in the .c file!
extern const char *context_strings[];
