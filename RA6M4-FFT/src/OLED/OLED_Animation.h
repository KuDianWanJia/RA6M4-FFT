#ifndef __OLED_ANIMATION_H
#define __OLED_ANIMATION_H

#include "stdlib.h"

#include "OLED_GFX.h"

#define MINDMAX 25

typedef enum
{
  OLED_OK       = 0x00U,
  OLED_ERROR    = 0x01U,
  OLED_BUSY     = 0x02U,
  OLED_IDLE     = 0x03U,
} OLED_STATUS;

void Motion_MindInit(void);
void Motion_Mind(void);
OLED_STATUS OLED_MovMind(uint8 Index);

#endif
