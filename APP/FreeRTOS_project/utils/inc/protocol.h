#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "stdint.h"
#define	STX  						0x5A
#define ETX							0xA5
#define MAX_RECV_DATA		64

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum RECV_STAT
{
  RECV_STAT_INIT = 0,
	RECV_STAT_LEN,
	RECV_STAT_CMD,
	RECV_STAT_FLAG,
	RECV_STAT_DAT,
	RECV_STAT_CRC,
	RECV_STAT_END,
	RECV_STAT_MAX
}RECV_STAT_E;

typedef enum RADAR_CMD
{
	CMD_NONE,
	CMD_STATE,
}cmd_cfg;

typedef enum RADAR_ERROR
{
	E_ERR_NONE,
	E_ERR_UNKOWN_CMD,	
	E_ERR_PARA_ERR,	
	E_ERR_LEN_ERR,		
	E_ERR_MAX
}error_cfg;


typedef struct
{
	RECV_STAT_E stat;
	uint8_t   stx;
	uint8_t 	len;
	uint8_t	  cmd;
	uint8_t	  flag;
	uint8_t		buff[MAX_RECV_DATA];
	uint8_t		crc;
	uint8_t   index;
	uint8_t 	last_data;
}recv_info_t;

void pro_dis(void);
#endif
