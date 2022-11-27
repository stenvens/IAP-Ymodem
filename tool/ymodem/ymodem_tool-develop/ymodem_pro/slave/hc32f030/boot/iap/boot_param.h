#ifndef _BOOT_PARAM_H_
#define _BOOT_PARAM_H_
#include <stdint.h>


//�洢�豸�ṹ���С
#define BOOT_PARAM_SIZE				(4*4)

//�洢�豸�ṹ��
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

//�����ⲿ����
extern boot_param_t boot_param;

//��������
void boot_param_write(void);
void boot_param_read(void);


#endif





