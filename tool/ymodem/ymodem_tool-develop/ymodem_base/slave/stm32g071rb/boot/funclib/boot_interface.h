#ifndef _BOOT_INTERFACE_H_
#define _BOOT_INTERFACE_H_
#include <stdint.h>
#include <stdbool.h>


/************************** IAP 数据相关参数 **************************/
// 系统默认的起始ROM和RAM地址
#define BASE_OF_ROM					(0x08000000UL)

//每个FLash扇区大小
#define MCU_FLASH_PAGE_SIZE    		2048

/************************************************************
*应用程序由Bootloader引导再启动
*------------------------------------------------------------
*内部Flash分区:
*  6kB boot |  4k param |  118k app1
*------------------------------------------------------------
* 对于APP工程的设置：
*   MDK的Target -> IROM1设置起始地址0x2800,Size设置为0x1D800
*   修改system_stm32g0xx.c下的VECT_TAB_OFFSET值为0x2800
************************************************************/

// Bootloader代码区域
#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
#define BLOCK_BOOTLOADER_NUM			(3UL) // 3页 3*2k=6k空间
#define BLOCK_BOOTLOADER_SPACE			(MCU_FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)

// 系统参数区域	
#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)
#define BLCOK_SYS_PARAM_NUM				(2UL)	// 2页 2*2k=4k空间
#define BLCOK_SYS_PARAM_SPACE			(MCU_FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	

// APP1代码区域
#define BLOCK_APP1_START				(BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)
#define BLOCK_APP1_NUM					(59UL) // 59页 59*2k=118k空间
#define BLCOK_APP1_SPACE				(MCU_FLASH_PAGE_SIZE*BLOCK_APP1_NUM)	


#endif

