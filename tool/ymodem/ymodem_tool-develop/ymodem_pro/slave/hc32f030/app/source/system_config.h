#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_
#include <stdio.h>


#define SOFTWARE_VERSION_PRODUCT            "Ymodem_App_"
#define SOFTWARE_VERSION_FORMAT(VERSION)    (SOFTWARE_VERSION_PRODUCT #VERSION) 
#define SOFTWARE_VERSION                    SOFTWARE_VERSION_FORMAT(001) //输入当前版本号

#endif

