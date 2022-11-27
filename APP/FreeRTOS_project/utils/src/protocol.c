/*****************************************************************
 * name:	prptocol.c
 * function:	协议解析
 * author:	sxh
 * date:	2022-11-23
 * version:	1.0.0
  -------------------------------------------------------------------------
 |  帧头 | 数据长度 | 指令码  | 读写标志位 | 数据 | 校验和的低八位| 帧尾   |
 |________________________________________________________________
 | 0x5A  |     n    |   code  |   0/1      | data |      crc_l    | 0xA5   |
 |__________________________________________________________________________
  n = 3+ndata   crc = 数据长度+指令码+读写标志位+ndata
*****************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "protocol.h"
#include "ring_buffer.h"
#include "bsp_flash.h"
RING_BUFF	recvBuff;
static recv_info_t recvInfo;
/**
  * @brief   计算校验和   
  * @note    Buf:要校验的数据 Len:要校验数据的长度 
  * @param   none
  * @retval 
  */
uint8_t CheckSum(uint8_t *Buf,uint8_t Len)
{
  uint8_t i =0;
  uint8_t sum =0;
  uint8_t checksum =0;
  
  for(i=0; i<Len; i++)
  {
    sum += *Buf++;
  }
  checksum = sum &0xff;
  return checksum;
}


//接收数据按协议解码,info:协议状态,dat:新接收的数据
bool RecvDataDecode(recv_info_t *info,uint8_t dat)
{

	bool result = FALSE;
	
	if(STX == dat && info->last_data == 0)
	{
		info->stat = RECV_STAT_INIT;
//		info->last_data = 0;
	}

	switch(info->stat)
	{
		case RECV_STAT_INIT:
			if(dat == STX)
			{
				info->stat++;
				info->len = 0;
				info->last_data++;
				info->stx = dat;
			}
			info->index = 0;
			break;

		case RECV_STAT_LEN:
			info->len = dat;
			info->index++;
			if(info->index >= 1)
			{
//				info->index = 0;
				info->cmd = 0;
				if(info->len < 4)
				{
					info->stat = RECV_STAT_INIT;
					info->last_data = 0;
					break;
				}
//				info->len -= 3;
				
				if(info->len > MAX_RECV_DATA)
				{
					info->stat = RECV_STAT_INIT;
					info->last_data = 0;
					break;
				}
				info->stat++;
			}
			break;
		case RECV_STAT_CMD:
			info->cmd  = dat;
			info->index++;
			if(info->index >= 2)
			{
				info->index = 0;
				info->stat++;
			}	
			else
			{
				info->last_data = 0;
			}
			break;
		case RECV_STAT_FLAG:
			if(dat == 1 || dat == 0)
				info->stat++;
			else
			{
				info->stat = RECV_STAT_INIT;
				info->last_data = 0;
			}
				
		break;
			
		case RECV_STAT_DAT:
			info->buff[info->index] = dat;
			info->index++;
			if(info->index >= (info->len-3))
			{
				info->stat++;
				info->index = 0;
			}
			else
			{
				info->stat = RECV_STAT_INIT;
			  info->last_data = 0;
			}
			break;
			
		case RECV_STAT_CRC:
				info->crc = CheckSum((uint8_t*)&info->stx,1+info->len);
				if(info->crc == dat)
				{
					info->stat++;
				}
				else
				{
					info->stat = RECV_STAT_INIT;
				  info->last_data = 0;
				}
			break;
			
		case RECV_STAT_END:
			if(ETX == dat)
			{
				result = TRUE;
			}
			else
			info->index = 0;		
			info->stat = RECV_STAT_INIT;
			break;
			
		default:
			info->index = 0;		
			info->stat = RECV_STAT_INIT;
			break;
	}
	return result;
}

uint32_t updata_flag = 1;

void pro_dis(void)
{
	uint8_t dat;
	bool isRet;
	recv_info_t *info = &recvInfo;
	
	while(Ring_BuffRead(&recvBuff, &dat))
	{
		if(RecvDataDecode(info,dat))
		{
			isRet = TRUE;
			switch (info->cmd)
			{
				case 0:
						bsp_flash_erase(PAR_ADDRESS,1);
						bsp_flash_write(PAR_ADDRESS,(uint8_t *)&updata_flag,4);
				    NVIC_SystemReset();
				
					break;

				default:
					info->len = 0;
					break;
			}
			if(isRet)
			{
//				SendDataEncode(info->cmd,err,info->buff,info->len);
			}
		}
	}
}
