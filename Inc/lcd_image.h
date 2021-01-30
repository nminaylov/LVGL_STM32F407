#ifndef __LCD_IMAGE
#define __LCD_IMAGE

#include "stm_lib.h"

typedef struct
{
    const uint8_t *data;
    uint16_t width;
    uint16_t height;
} tImage;

#endif /* __LCD_IMAGE */
