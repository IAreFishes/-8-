#ifndef __EXTI_H
#define __EXTI_H
#include "includes.h"
void EXTI_AllConfiguration(void);
void EXTI_Configuration(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource, EXTITrigger_TypeDef EXTI_Trigger_X,FunctionalState state);
#endif
