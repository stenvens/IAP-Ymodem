#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H
#include <stdbool.h>
#include <stdint.h>

//º¯ÊıÉùÃ÷
uint8_t bsp_flash_read_byte(uint32_t FlashAddr);
bool bsp_flash_write_byte(uint32_t FlashAddr, uint8_t Data);
uint32_t bsp_flash_read_word(uint32_t FlashAddr);
bool bsp_flash_write_word(uint32_t FlashAddr, uint32_t Data);
bool bsp_flash_erase_block(uint32_t erase_start_address, uint32_t block_num);
void boot_interface_write_flash(uint32_t address, uint8_t *buf, uint32_t length);

#endif

