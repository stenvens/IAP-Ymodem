#ifndef _BOOT_INTERFACE_H_
#define _BOOT_INTERFACE_H_
#include <stdint.h>
#include <stdbool.h>


//MDK的Target -> IROM1设置起始地址0x1C00,Size设置为0xE400
//修改startup_hc32f030.c下的new_vect_table值为0x1C00


/************************** IAP 数据相关参数 **************************/
// 系统默认的起始ROM和RAM地址
#define BASE_OF_ROM					(0x00000000)

//每个FLash扇区大小
#define MCU_FLASH_PAGE_SIZE    		512

/************************************************************
*应用程序由Bootloader引导再启动
*------------------------------------------------------------
*内部Flash分区:
*  6.5k boot |  0.5k param |  57k app1
************************************************************/

// Bootloader代码区域
#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
#define BLOCK_BOOTLOADER_NUM			(13UL) // 13页 13*0.5k=6.5k空间
#define BLOCK_BOOTLOADER_SPACE			(MCU_FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)

// 系统参数区域	
#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)
#define BLCOK_SYS_PARAM_NUM				(1UL)	// 1页 1*0.5=0.5k空间
#define BLCOK_SYS_PARAM_SPACE			(MCU_FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	

// APP1代码区域
#define BLOCK_APP1_START				(BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)
#define BLOCK_APP1_NUM					(114UL) // 114页 114*0.5=57k空间
#define BLCOK_APP1_SPACE				(MCU_FLASH_PAGE_SIZE*BLOCK_APP1_NUM)	

#endif

