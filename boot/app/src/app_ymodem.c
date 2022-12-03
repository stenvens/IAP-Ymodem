#include "board.h"
static uint8_t FileName[1024];
static uint8_t jump_flag = 0;

/*
*********************************************************************************************************
*	?? ?? ??: UpdateCRC16
*	???????: ??¦Ì????CRC??? crcIn ??????????????????CRC
*	??    ??: crcIn ?????CRC??????
*             byte  ?????????
*	?? ?? ?: ??
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
*	?? ?? ??: Cal_CRC16
*	???????: ????????????CRC
*	??    ??: data  ????
*             size  ???????
*	?? ?? ?: CRC??????
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
*	?? ?? ??: Receive_Byte
*	???????: ????????????????         
*	??    ?¦²?c  ???
*             timeout  ??????
*	?? ?? ?: 0 ???????? -1 ???????
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
*	?? ?? ??: Send_Byte
*	???????: ??????????????         
*	??    ?¦²?c  ???
*	?? ?? ?: 0
*********************************************************************************************************
*/
static uint32_t Send_Byte (uint8_t c)
{
	USART_SendData(UART_NUM,c);
	return 0;
}

/*
*********************************************************************************************************
*	?? ?? ??: Receive_Packet
*	???????: ???????????        
*	??    ?¦²?data ????
*             length ?????§³
*             timeout  0 ???????
*                      -1 ????????????
*                      >0 ?????????
*	?? ?? ?: 0  ????????
*             -1 ?????????????????
*             1  ??????
*********************************************************************************************************
*/
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
	uint16_t i, packet_size;
	uint8_t c;
	uint16_t crc;
	
	*length = 0;

	
	/* ?????????? */
	if (Receive_Byte(&c, timeout) != 0)
	{
		return -1;
	}
	
	switch (c)
	{
		/* SOH???????????128??? */
		case SOH:
			packet_size = PACKET_SIZE;
			break;
		
		/* STX???????????1k??? */
		case STX:
			packet_size = PACKET_1K_SIZE;
			break;
		
		/* ??????? end of transmission */
		case EOT:
			return 0;
		
		/* ??????????CA?????????? */
		case CA:
			/* ?????????????CA??? */
			if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
			{
				*length = -1;
				return 0;
			}
			/* ???????CA??? */
			else
			{
				return -1;
			}
		
		/* ?????????? */
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
	
	/* ??PACKET_SEQNO_COMP_INDEX??????2???????PACKET_SEQNO_INDEX??????1????????? */
	if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
	{
		return -1;
	}
	
	/* ????CRC */
	crc = data[ packet_size + PACKET_HEADER ] << 8;
	crc += data[ packet_size + PACKET_HEADER + 1 ];
	if (Cal_CRC16(&data[PACKET_HEADER], packet_size) != crc)
	{
		return -1;
	}	
	
	/* ?????????? */
	*length = packet_size;
	return 0;
}

/*
*********************************************************************************************************
*	?? ?? ??: Receive_Packet
*	???????: ????ymodem§¿?????????       
*	??    ??: buf ????????
*	?? ?? ?: ?????§³
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

	/* ?????flash??????? */
	flashdestination = appadr;

	/* ?????????????flash??? */
	for (session_done = 0, errors = 0, session_begin = 0; ;)
	{
		for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
		{
			switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
			{
				/* ????0????????? */
				case 0:
					errors = 0;
					switch (packet_length)
					{
						/* ???????????? */
						case - 1:
							Send_Byte(ACK);
							return 0;
						
						/* ??????? */
						case 0:
							Send_Byte(ACK);
							file_done = 1;
							break;
						
						/* ???????? */
						default:
							if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
							{
								Send_Byte(NAK);
							}
							else
							{
								if (packets_received == 0)
								{
									/* ?????????? */
									if (packet_data[PACKET_HEADER] != 0)
									{
										/* ???????? */
										for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
										{
											FileName[i++] = *file_ptr++;
										}
										/* ??????¦Â??????? */
										FileName[i++] = '\0';
										
										/* ????????§³ */
										for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
										{
											file_size[i++] = *file_ptr++;
										}
										file_size[i++] = '\0';
										
										/* ???????§³???????????????????? */
										Str2Int(file_size, &size);

										
										/* ????????§³????flash???? */
										if (size > (1024*1024*2 + 1))
										{
											/* ??????? */
											Send_Byte(CA);
											Send_Byte(CA);
											return -1;
										}

										/* ?????????flash */
										SectorCount = size/(FLASH_PAGESIZE);
										SectorRemain = size%(FLASH_PAGESIZE);	
										
										for(i = 0; i < SectorCount; i++)
										{
											ucState = bsp_flash_erase((uint32_t)(flashdestination + i*FLASH_PAGESIZE),1);
											if(ucState == 1)
											{
												/* ??????? */
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
									/* ???????????????????????????????????? */
									else
									{
										Send_Byte(ACK);
										file_done = 1;
										session_done = 1;
										break;
									}
								}
								
								/* ????? */
								else
								{
									/* ??????? */
									memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
									
									/* ??????? */
									ucState = bsp_flash_write((uint32_t)(flashdestination + TotalSize),  (uint8_t *)buf_ptr, packet_length);
									TotalSize += packet_length;
									
									/* ????????0??????????? */
									if(ucState != 0)
									{
										/* ??????? */
										Send_Byte(CA);
										Send_Byte(CA);
										return -2;
									}
									ClearRingBuf();
									Send_Byte(ACK);
								}
								/* ????????????? */
								packets_received ++;
								session_begin = 1;
							}
					}
					break;
				
				/* ?????????? */
				case 1:
					Send_Byte(CA);
					Send_Byte(CA);
					return -3;
				
				/* ???? */
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
