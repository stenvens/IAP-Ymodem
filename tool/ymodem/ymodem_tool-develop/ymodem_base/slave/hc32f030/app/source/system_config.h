#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_
#include <stdio.h>


#define SOFTWARE_VERSION                    "V0.01"
#define HARDWARE_BOARD                    	"HC32F030J8TA"


#define log(...)  \
	do{\
		printf(__VA_ARGS__);\
	}while(0)

#endif

