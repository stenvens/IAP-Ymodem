#ifndef _BOOT_IAP_H_
#define _BOOT_IAP_H_
#include "main.h"

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.
void iap_load_app(uint32_t appxaddr);			//ִ��flash�����app����
void iap_reboot(void);

#endif

