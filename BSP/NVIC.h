#ifndef __NVIC_H
#define __NVIC_H
#include "stm32f10x.h"
void NVIC_Configuration(uint8_t IRQChannel,uint8_t PreemptionPriority, uint8_t SubPriority);
void NVIC_Configuration_All(void);
#endif
