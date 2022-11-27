#ifndef _BOOT_INTERFACE_H_
#define _BOOT_INTERFACE_H_
#include <stdint.h>
#include <stdbool.h>


/************************** IAP ������ز��� **************************/
// ϵͳĬ�ϵ���ʼROM��RAM��ַ
#define BASE_OF_ROM					(0x08000000UL)

//ÿ��FLash������С
#define MCU_FLASH_PAGE_SIZE    		2048

/************************************************************
*Ӧ�ó�����Bootloader����������
*------------------------------------------------------------
*�ڲ�Flash����:
*  6kB boot |  4k param |  118k app1
*------------------------------------------------------------
* ����APP���̵����ã�
*   MDK��Target -> IROM1������ʼ��ַ0x2800,Size����Ϊ0x1D800
*   �޸�system_stm32g0xx.c�µ�VECT_TAB_OFFSETֵΪ0x2800
************************************************************/

// Bootloader��������
#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
#define BLOCK_BOOTLOADER_NUM			(3UL) // 3ҳ 3*2k=6k�ռ�
#define BLOCK_BOOTLOADER_SPACE			(MCU_FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)

// ϵͳ��������	
#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START + BLOCK_BOOTLOADER_SPACE)
#define BLCOK_SYS_PARAM_NUM				(2UL)	// 2ҳ 2*2k=4k�ռ�
#define BLCOK_SYS_PARAM_SPACE			(MCU_FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	

// APP1��������
#define BLOCK_APP1_START				(BLCOK_SYS_PARAM_START + BLCOK_SYS_PARAM_SPACE)
#define BLOCK_APP1_NUM					(59UL) // 59ҳ 59*2k=118k�ռ�
#define BLCOK_APP1_SPACE				(MCU_FLASH_PAGE_SIZE*BLOCK_APP1_NUM)	


#endif

