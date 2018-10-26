#ifndef __GPIO_H
#define __GPIO_H
#include "includes.h"

void GPIO_Config_All(void);
void GPIO_Config(GPIO_TypeDef *GPIOx, uint16_t PINX, GPIOMode_TypeDef GPIO_MODE);
#endif
