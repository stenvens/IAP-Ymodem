#ifndef _BOOT_PARAM_H_
#define _BOOT_PARAM_H_
#include <stdint.h>


//存储设备结构体大小
#define BOOT_PARAM_SIZE				(4*4)

//存储设备结构体
typedef union 
{
	struct
	{
		uint32_t bootloader_version;
		uint32_t usart_iap_status;
		uint32_t remote_update_status;
		uint32_t remote_update_file_size;
	}tab;
	uint8_t array[BOOT_PARAM_SIZE];
}boot_param_t; 

//变量外部声明
extern boot_param_t boot_param;

//函数声明
void boot_param_write(void);
void boot_param_read(void);


#endif





