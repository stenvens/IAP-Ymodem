#ifndef _BOOT_IAP_H_
#define _BOOT_IAP_H_
#include "main.h"

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
void iap_load_app(uint32_t appxaddr);			//执行flash里面的app程序
void iap_reboot(void);

#endif

