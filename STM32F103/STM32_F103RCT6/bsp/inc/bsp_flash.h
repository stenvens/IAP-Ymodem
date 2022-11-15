#ifndef __BSP_FLASH_H_
#define __BSP_FLASH_H_
#include "config.h"
uint8_t  bsp_flash_erase(uint32_t  address,uint32_t page_num);
uint8_t bsp_flash_write(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);
#endif

