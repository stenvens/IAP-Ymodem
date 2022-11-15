#ifndef __BSP_SYSTICK_H_
#define __BSP_SYSTICK_H_
#include "config.h"
uint32_t  get_tickcnt(void);
uint32_t SysTick_init(uint32_t ticks);
#endif

