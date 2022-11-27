#include "uart_config.h"
#include "bsp_uart.h"
#include "boot_iap.h"
#include "system_config.h"


//��������
void help_uart_callback(uint8_t *data, uint16_t size);
void reboot_uart_callback(uint8_t *data, uint16_t size);
void get_device_version_uart_callback(uint8_t *data, uint16_t size);

config_uart_t config_uart; //���ô���

struct debug_cmd_t
{
    char *indentify_cmd;
    void (*func)(uint8_t *data, uint16_t size);
};

static struct debug_cmd_t uart_cmd_table[] = 
{
	{"help",				help_uart_callback},
	{"reboot",				reboot_uart_callback},
	{"get_device_version",	get_device_version_uart_callback},
};

/*********************************************************************
* @brief  ����ѯ����
*********************************************************************/
void help_uart_callback(uint8_t *data, uint16_t size)
{
	char buf[50] = {0};
	sprintf(buf, "cmd list:\r\n");
	uart_config_send(buf, strlen(buf));
	
	for(uint8_t i=0; i<(sizeof(uart_cmd_table)/sizeof(struct debug_cmd_t)); i++)
	{
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%s\r\n", uart_cmd_table[i].indentify_cmd);
		uart_config_send(buf, strlen(buf));
	}
}

/*********************************************************************
* @brief  ����������
*********************************************************************/
void reboot_uart_callback(uint8_t *data, uint16_t size)
{
	iap_reboot();
}

/*********************************************************************
* @brief  ����ȡ�豸�汾��
*********************************************************************/
void get_device_version_uart_callback(uint8_t *data, uint16_t size)
{
	char buf[50] = {0};
	sprintf(buf, "get_device_version=%s (%s)\r\n", SOFTWARE_VERSION, HARDWARE_BOARD);
	uart_config_send(buf, strlen(buf));
}

/*********************************************************************
* @brief  ����ȡ��ǰ���ڻ��������յ������ݳ���
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ���������ݳ���
*********************************************************************/
static uint16_t get_uart_recv_length(void)
{
	return usart2_rx_cnt;
}

/*********************************************************************
* @brief  ����ȡ���ڻ�������ָ��
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval �����ڻ�������ָ��
*********************************************************************/
static uint8_t* get_uart_recv_buf(void)
{
	return (uint8_t*)&usart2_rx_buf;
}

/*********************************************************************
* @brief  ����ȡ������������						 
*********************************************************************/
static void get_uart_config_data(void)
{
	config_uart.recv_length = get_uart_recv_length();
	memcpy(config_uart.recv_buf, get_uart_recv_buf(), config_uart.recv_length);
}

/*********************************************************************
* @brief  ��������ڴ�������						 
*********************************************************************/
static void clear_uart_data(void)
{
	memset((uint8_t *)&config_uart, 0x00, sizeof(config_uart_t));
	
	memset(usart2_rx_buf, 0x00, USART2_BUF_MAXIMUM);
	usart2_rx_cnt = 0;
}

/*********************************************************************
* @brief  �����ô��ڷ���
*---------------------------------------------------------------------
* @param  ��char *buf           ,   ����
*           uint16_t length     ,   ���ݳ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void uart_config_send(char *buf, uint16_t length)
{
  	memcpy(config_uart.send_buf, buf, length);
	config_uart.send_length = length;
	
	bsp_usart1_send(config_uart.send_buf, config_uart.send_length);
}

/*********************************************************************
* @brief  ���������õ����ݴ���	 
*********************************************************************/
static void uart_config_data_proc(void)
{
  	char *p;
	
	for(uint8_t i=0; i<(sizeof(uart_cmd_table)/sizeof(struct debug_cmd_t)); i++)
	{
		p = strstr((char*)config_uart.recv_buf, uart_cmd_table[i].indentify_cmd);
		if(p != NULL) 
		{
			uart_cmd_table[i].func(config_uart.recv_buf, config_uart.recv_length);
			return;
		}
	}
}

/*********************************************************************
* @brief  �������������ݴ���
*********************************************************************/
void uart_config_transmition(void)
{
	get_uart_config_data();
	uart_config_data_proc();
	clear_uart_data();
}

/*********************************************************************
* @brief  ���������ý��մ��� (����ѭ��xx msִ��һ��)
*********************************************************************/
void uart_recv_proc(void)
{
	uint16_t uart_recv_cnt = get_uart_recv_length();
	static uint16_t last_uart_recv_cnt = 0;
	
	if(uart_recv_cnt != 0)
	{		
		if(uart_recv_cnt == last_uart_recv_cnt)
		{             
			uart_config_transmition();
		}
		
		last_uart_recv_cnt = uart_recv_cnt;
	}
}

