#ifndef _DELAY_H
#define _DELAY_H
#include "ddl.h"

#define  DI()      { __disable_irq();  }
#define  EI()      { __enable_irq();   }

/* º¯ÊıÉùÃ÷ */
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);


#endif

