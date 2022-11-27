/********************************************************************************************************
*   ��Ŀ���� : IAP_Ymodem_pro (Bootloader)
*   ƽ    ̨ : HC32F030J8TA
*	��    �� : biglu
*	����˵�� : 
*				-1-	USART1(PA2/PA3)��ΪͨѶ����,��Ϊ����λ�����������á�
* 
*				-2-	����Э�����Ymodem��֧��128Byte��1KByte��
*					Ymodem���ֽ�ȡ�޸���ST��IAP���ӡ�
*
*				-3-	�����޸�λ�ã�boot_interface.h��
*
*				-4-	�ϵ��600ms�����յ������ź�'st'�������Ymodem�������̡�
*					��δ�յ��źţ��������ת�жϡ�
*
********************************************************************************************************/


//lib files
#include "ddl.h"
#include "flash.h"
#include "hal_delay.h"

//bsp files
#include "bsp_uart.h"

//iap files
#include "menu.h"
#include "boot_interface.h"
#include "boot_param.h"
#include "boot_iap.h"


//��������
extern void FlashInt(void);

/*********************************************************************
* @brief  �����ڽ�������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,		���ջ�����
*			uint16_t size		,		ָ�����ճ���
*			uint32_t timeout	,		���ճ�ʱʱ��
*---------------------------------------------------------------------
* @retval ��0,���ճ�ʱ			/		1,���ճɹ�
*********************************************************************/
uint8_t uart_receive(uint8_t *buf, uint16_t size, uint32_t timeout)
{
	uint32_t tickstart = 0;

	tickstart = HAL_GetTick();
	
	for(uint16_t i=0; i<size; i++)
	{
		while(0 == M0P_UART1->ISR_f.RC) //�ȴ�RC��־
		{
			if((timeout == 0)||((HAL_GetTick() - tickstart ) > timeout))return 0; //��ʱ
		}
		M0P_UART1->ICR_f.RCCF = 0; //���RC��־
		buf[i] = M0P_UART1->SBUF_f.DATA;
	}
		
	return 1;
}

/*********************************************************************
* @brief  �����ڷ�������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,		���ջ�����
*			uint16_t size		,		ָ�����ճ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void uart_send(uint8_t *buf, uint16_t size)
{
	bsp_uart1_send(buf, size);
}

/*********************************************************************
* @brief  �������ʼ��
*********************************************************************/
void bsp_init(void)
{
	//�ϵ�Ĭ��4M-�ڲ�RC
	Flash_Init(FlashInt, 1, TRUE); //FLASH��ʼ��
	
	//��ʼ���δ�ʱ��Դ
	HAL_InitTick();
	
	//IAP����
	bsp_uart1_init(); 
}

/*********************************************************************
* @brief  �����跴��ʼ��
*********************************************************************/
void bsp_deinit(void)
{
	//iap usart
	bsp_uart1_deinit();
	
	HAL_DeInitTick();
}

/*********************************************************************
* @brief  ��ȷ����ת����
*********************************************************************/
void check_jump_condition(void)
{
	/*
	ȷ����תAPP����:
	1. δ����������ͨѶ�ϣ�appδ������д�������кϷ���app --> ��ת��app
	2. δ����������ͨѶ�ϣ�appδ������д������û�кϷ���app --> ����ת���ȴ�
	3. δ����������ͨѶ�ϣ�app����д�� --> ����ת���ȴ�
	*/
	if(boot_interface_check_app_erased() == true) //�������û������ (����һ����¼��HEX�ļ���ֻ��¼��Bootloader)
	{    
		if(boot_interface_check_app_existed() == true) //��ת����1
		{   
			printf("app exist, jump to app... (app never erased & valid app)\r\n");
			bsp_deinit();
			__disable_irq(); //�ر������ж�
			iap_load_app(BLOCK_APP1_START);
		}
		else //��ת����2
		{
			printf("app not exist, wait... (app never erased & no valid app)\r\n");
		}
	}
	else //��ת����3
	{
		printf("app not exist, wait... (app erased)\r\n");
	}
}

/*********************************************************************
* @brief  ��ȷ�������ź�
*---------------------------------------------------------------------
* @retval ��0,δ�յ�		/		1,�յ�	
*********************************************************************/
uint8_t check_update_signal(void)
{
	uint8_t buf[2] = {0};
	uint32_t start_tick = HAL_GetTick();
	
	while(HAL_GetTick() <= (start_tick + 600))
	{
		if(uart_receive(buf, 2, 50) == 1)
		{
			if(buf[0] == 's' && buf[1] == 't')
			{
				return 1;
			}
		}
	}
	
	return 0;
}

/*********************************************************************
* @brief  ���ȴ������ź�
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ��0	,	һ��ʱ����δ�յ������ź�
*			1	,	�յ������ź�
*********************************************************************/
uint8_t wait_reboot_signal(void)
{
	uint8_t ret = 0;
	uint8_t cnt = 0;
	uint8_t char1 = 0;
	char *dest_buf = "reboot";
	
	while(cnt < strlen(dest_buf))
	{
		if(uart_receive(&char1, 1, 50) == 1 && char1 == dest_buf[cnt])
		{
			ret = 1;
		}
		else
		{
			ret = 0;
			break;
		}
		
		cnt++;
	}
	
	return ret;
}

/*********************************************************************
* @brief  ��������
*********************************************************************/
int32_t main(void)
{
	//��ʼ��
	bsp_init();
	boot_param_read();
	
	//�ȴ������ź�
	if(check_update_signal() == 0)
	{
		//�������ºţ���ȷ����ת����
		check_jump_condition();
	}

	//Ymodem��������
	Main_Menu();
	
    while(1)
	{
		//�ȴ������ź����ٴν�������ģʽ
		if(wait_reboot_signal() == 1)
		{
			iap_reboot();
		}
	}
}

