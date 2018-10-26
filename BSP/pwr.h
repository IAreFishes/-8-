#ifndef __PWR_H
#define __PWR_H
#include "includes.h"
typedef enum
{ 
    POWER_NORMAL=0,
	POWER_SLEEP,
	POWER_STOP,
	POWER_STANDBY
}PWRER_MODE_TypeDef;

void Pwr_Ctrl(PWRER_MODE_TypeDef mode);
#endif
