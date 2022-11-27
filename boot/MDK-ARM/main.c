#include "board.h"

static uint8_t UartRecBuf[248] ={0};
static uint32_t code_size = 0;


static uint32_t updata_flag = 0x0;

int main(void)
{
	board_init();
	bsp_flash_read(0x0803F000,(uint8_t *)&updata_flag,4);
	//�ж�������־�Ƿ���1
	if(updata_flag == 1)
	{
		/* Ymodem���� */
		code_size = Ymodem_Receive(UartRecBuf, APP_ADDRESS);
		if(code_size != 0)
		{
			updata_flag = 0;
		  bsp_flash_write(0x0803F000,(uint8_t *)&updata_flag,4);
			jump_to_app();
		}
		else
		{
			updata_flag = 0;
		  bsp_flash_write(0x0803F000,(uint8_t *)&updata_flag,4);
			jump_to_app();
		
		}	
	}
	else
	{
	  jump_to_app();
	}
  

	while(1)
	{

		
	}
}
