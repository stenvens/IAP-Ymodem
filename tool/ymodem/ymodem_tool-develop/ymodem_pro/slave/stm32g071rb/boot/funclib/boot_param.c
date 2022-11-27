#include "boot_param.h"
#include "boot_interface.h"
#include "bsp_flash.h"


boot_param_t boot_param;

void boot_param_write(void)
{	
	uint32_t i = 0;
	uint32_t length = 0, start_address = BLCOK_SYS_PARAM_START;
	uint64_t data = 0;
	
	bsp_flash_erase_block(BLCOK_SYS_PARAM_START, 1);
	
	//长度非8字节倍数时则补齐新的4字节
	if(BOOT_PARAM_SIZE%8 != 0)
	{
		length = (length/8)*8 + 8;
	}

	for (i=0; i<(length/8); i++)
	{	
		//小端格式存放
		data = boot_param.array[i*8 + 0];
		data |= boot_param.array[i*8 + 1] << 8;
		data |= boot_param.array[i*8 + 2] << 16;
		data |= boot_param.array[i*8 + 3] << 24;
		bsp_flash_write_double_word(start_address, data);
		start_address += 8;
	}	
}

void boot_param_read(void)
{
	for(uint16_t i=0; i<BOOT_PARAM_SIZE; i++)
	{
		boot_param.array[i] = bsp_flash_read_byte(BLCOK_SYS_PARAM_START + i);
	}
}

