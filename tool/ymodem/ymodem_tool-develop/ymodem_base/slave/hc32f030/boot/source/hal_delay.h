#ifndef _HAL_DELAY_H_
#define _HAL_DELAY_H_
#include <stdint.h>


void HAL_InitTick(void);
void HAL_DeInitTick(void);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(volatile uint32_t Delay);

#endif

