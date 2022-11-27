#include "board.h"
static uint8_t FileName[1024];
static uint8_t jump_flag = 0;

/*
*********************************************************************************************************
*	函 数 名: UpdateCRC16
*	功能说明: 上次计算的CRC结果 crcIn 再加上一个字节数据计算CRC
*	形    参: crcIn 上一次CRC计算结果
*             byte  新添加字节
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t in = byte | 0x100;

  do
  {
	crc <<= 1;
	in <<= 1;
	if(in & 0x100)
		++crc;
	if(crc & 0x10000)
		crc ^= 0x1021;
  }while(!(in & 0x10000));

  return crc & 0xffffu;
}

/*
*********************************************************************************************************
*	函 数 名: Cal_CRC16
*	功能说明: 计算一串数据的CRC
*	形    参: data  数据
*             size  数据长度
*	返 回 值: CRC计算结果
*********************************************************************************************************
*/
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
	uint32_t crc = 0;
	const uint8_t* dataEnd = data+size;

	while(data < dataEnd)
		crc = UpdateCRC16(crc, *data++);

	crc = UpdateCRC16(crc, 0);
	crc = UpdateCRC16(crc, 0);

	return crc&0xffffu;
}

/*
*********************************************************************************************************
*	函 数 名: Receive_Byte
*	功能说明: 接收发送端发来的字符         
*	形    参：c  字符
*             timeout  溢出时间
*	返 回 值: 0 接收成功， -1 接收失败
*********************************************************************************************************
*/
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{

		if (GetRingBufchar(c,timeout) != 1)
		{
			return 1;
		}
		
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: Send_Byte
*	功能说明: 发送一个字节数据         
*	形    参：c  字符
*	返 回 值: 0
*********************************************************************************************************
*/
static uint32_t Send_Byte (uint8_t c)
{
	USART_SendData(UART_NUM,c);
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: Receive_Packet
*	功能说明: 接收一包数据        
*	形    参：data 数据
*             length 数据大小
*             timeout  0 传输结束
*                      -1 发送端终止传输
*                      >0 数据包长度
*	返 回 值: 0  正常返回
*             -1 时间溢出或数据包错误
*             1  用户终止
*********************************************************************************************************
*/
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
	uint16_t i, packet_size;
	uint8_t c;
	uint16_t crc;
	
	*length = 0;

	
	/* 接收一个字符 */
	if (Receive_Byte(&c, timeout) != 0)
	{
		return -1;
	}
	
	switch (c)
	{
		/* SOH表示数据区有128字节 */
		case SOH:
			packet_size = PACKET_SIZE;
			break;
		
		/* STX表示数据区有1k字节 */
		case STX:
			packet_size = PACKET_1K_SIZE;
			break;
		
		/* 传输结束 end of transmission */
		case EOT:
			return 0;
		
		/* 连续的两个CA信号终止传输 */
		case CA:
			/* 收到两个连续的CA信号 */
			if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
			{
				*length = -1;
				return 0;
			}
			/* 只收到一个CA信号 */
			else
			{
				return -1;
			}
		
		/* 用户终止传输 */
		case ABORT1:
		case ABORT2:
			return 1;
		
		default:
			return -1;
	}
	
	*data = c;
	for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
	{
		if (Receive_Byte(data + i, timeout) != 0)
		{
			return -1;
		}
	}
	
	/* 第PACKET_SEQNO_COMP_INDEX（数字2）字节是PACKET_SEQNO_INDEX（数字1）字节的反码 */
	if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
	{
		return -1;
	}
	
	/* 计算CRC */
	crc = data[ packet_size + PACKET_HEADER ] << 8;
	crc += data[ packet_size + PACKET_HEADER + 1 ];
	if (Cal_CRC16(&data[PACKET_HEADER], packet_size) != crc)
	{
		return -1;
	}	
	
	/* 数据区长度 */
	*length = packet_size;
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: Receive_Packet
*	功能说明: 按照ymodem协议接收数据       
*	形    参: buf 数据首地址
*	返 回 值: 文件大小
*********************************************************************************************************
*/
uint32_t TotalSize = 0;
int32_t Ymodem_Receive (uint8_t *buf, uint32_t appadr)
{
	uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
	int32_t i, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;
	uint32_t flashdestination;
	uint8_t ucState;
	uint32_t SectorCount = 0;
	uint32_t SectorRemain = 0;

	/* 初始化flash编程首地址 */
	flashdestination = appadr;

	/* 接收数据并进行flash编程 */
	for (session_done = 0, errors = 0, session_begin = 0; ;)
	{
		for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
		{
			switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
			{
				/* 返回0表示接收成功 */
				case 0:
					errors = 0;
					switch (packet_length)
					{
						/* 发送端终止传输 */
						case - 1:
							Send_Byte(ACK);
							return 0;
						
						/* 传输结束 */
						case 0:
							Send_Byte(ACK);
							file_done = 1;
							break;
						
						/* 接收数据 */
						default:
							if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
							{
								Send_Byte(NAK);
							}
							else
							{
								if (packets_received == 0)
								{
									/* 文件名数据包 */
									if (packet_data[PACKET_HEADER] != 0)
									{
										/* 读取文件名 */
										for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
										{
											FileName[i++] = *file_ptr++;
										}
										/* 文件名末尾加结束符 */
										FileName[i++] = '\0';
										
										/* 读取文件大小 */
										for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
										{
											file_size[i++] = *file_ptr++;
										}
										file_size[i++] = '\0';
										
										/* 将文件大小的字符串转换成整型数据 */
										Str2Int(file_size, &size);

										
										/* 检测文件大小是否比flash空间大 */
										if (size > (1024*1024*2 + 1))
										{
											/* 终止传输 */
											Send_Byte(CA);
											Send_Byte(CA);
											return -1;
										}

										/* 擦除用户区flash */
										SectorCount = size/(FLASH_PAGESIZE);
										SectorRemain = size%(FLASH_PAGESIZE);	
										
										for(i = 0; i < SectorCount; i++)
										{
											ucState = bsp_flash_erase((uint32_t)(flashdestination + i*FLASH_PAGESIZE),1);
											if(ucState == 1)
											{
												/* 终止传输 */
												Send_Byte(CA);
												Send_Byte(CA);
												return -2;
											}
										}
										
										if(SectorRemain)
										{
											bsp_flash_erase((uint32_t)(flashdestination + i*FLASH_PAGESIZE),1);
										}
										ClearRingBuf();
										Send_Byte(ACK);
										delay_ms(50);
										Send_Byte(CRC16);
									}
									/* 文件名数据包处理完，终止此部分，开始接收数据 */
									else
									{
										Send_Byte(ACK);
										file_done = 1;
										session_done = 1;
										break;
									}
								}
								
								/* 数据包 */
								else
								{
									/* 读取数据 */
									memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
									
									/* 扇区编程 */
									ucState = bsp_flash_write((uint32_t)(flashdestination + TotalSize),  (uint8_t *)buf_ptr, packet_length);
									TotalSize += packet_length;
									
									/* 如果返回非0，表示编程失败 */
									if(ucState != 0)
									{
										/* 终止传输 */
										Send_Byte(CA);
										Send_Byte(CA);
										return -2;
									}
									ClearRingBuf();
									Send_Byte(ACK);
								}
								/* 接收数据包递增 */
								packets_received ++;
								session_begin = 1;
							}
					}
					break;
				
				/* 用户终止传输 */
				case 1:
					Send_Byte(CA);
					Send_Byte(CA);
					return -3;
				
				/* 其它 */
				default:
					if (session_begin > 0)
					{
						errors ++;
					}
					
					if (errors > MAX_ERRORS)
					{
						Send_Byte(CA);
						Send_Byte(CA);
						return 0;
					}
					jump_flag++;
					if(jump_flag == 5)
					{
						file_done = 1;
						session_done = 1;
					}
						
					Send_Byte(CRC16);
					break;
			}
			
			if (file_done != 0)
			{
				break;
			}
		}
		
		if (session_done != 0)
		{
			break;
		}
	}
	
	return (int32_t)size;
}
