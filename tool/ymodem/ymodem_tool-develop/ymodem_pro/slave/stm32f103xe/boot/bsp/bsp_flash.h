#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_
#include "main.h"
#include <stdbool.h>

uint8_t bsp_flash_read_byte(uint32_t FlashAddr);
uint32_t bsp_flash_read_word(uint32_t FlashAddr);
bool bsp_flash_write_word(uint32_t WriteAddr, uint32_t data);
bool bsp_flash_erase_block(uint32_t erase_start_address, uint16_t erase_page_num);
void boot_interface_write_flash(uint32_t address, uint8_t *buf, uint32_t length);

#endif

