#include "boot_param.h"
#include "boot_interface.h"
#include "bsp_flash.h"


boot_param_t boot_param;

void boot_param_write(void)
{
	bsp_flash_erase_block(BLCOK_SYS_PARAM_START, 1);
	for(uint16_t i=0; i<BOOT_PARAM_SIZE; i++)
	{
		bsp_flash_write_byte(BLCOK_SYS_PARAM_START + i, boot_param.array[i]);
	}
}

void boot_param_read(void)
{
	for(uint16_t i=0; i<BOOT_PARAM_SIZE; i++)
	{
		boot_param.array[i] = bsp_flash_read_byte(BLCOK_SYS_PARAM_START + i);
	}
}

