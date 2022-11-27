#ifndef _BOOT_INTERFACE_H_
#define _BOOT_INTERFACE_H_
#include <stdint.h>
#include <stdbool.h>


//MDK��Target -> IROM1������ʼ��ַ0x1C00,Size����Ϊ0xE400
//�޸�startup_hc32f030.c�µ�new_vect_tableֵΪ0x1C00


/************************** IAP ������ز��� **************************/
// ϵͳĬ�ϵ���ʼROM��RAM��ַ
#define BASE_OF_ROM					(0x00000000)

//ÿ��FLash������С
#define MCU_FLASH_PAGE_SIZE    		512

/************************************************************
*Ӧ�ó�����Bootloader����������
*------------------------------------------------------------
*�ڲ�Flash����:
*  6.5k boot |  0.5k param |  57k app1
************************************************************/

// Bootloader��������
#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
#define BLOCK_BOOTLOADER_NUM			(13UL) // 13ҳ 13*0.5k=6.5k�ռ�
#define BLOCK_BOOTLOADER_SPACE			(MCU_FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)

// ϵͳ��������	
#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)
#define BLCOK_SYS_PARAM_NUM				(1UL)	// 1ҳ 1*0.5=0.5k�ռ�
#define BLCOK_SYS_PARAM_SPACE			(MCU_FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	

// APP1��������
#define BLOCK_APP1_START				(BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)
#define BLOCK_APP1_NUM					(114UL) // 114ҳ 114*0.5=57k�ռ�
#define BLCOK_APP1_SPACE				(MCU_FLASH_PAGE_SIZE*BLOCK_APP1_NUM)	

#endif

