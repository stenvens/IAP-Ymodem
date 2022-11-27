#ifndef __BOARD_H_
#define __BOARD_H_
#include "stdio.h"
#include "config.h"
#include "bsp_systick.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_flash.h"
#include "app_ymodem.h"
#include "common.h"
#include "app_iap.h"
#include "ring_buffer.h"
#include "string.h"
#if CM_BACKTRACE == 1
#include "cm_backtrace.h"
#endif
void board_init(void);
#endif

