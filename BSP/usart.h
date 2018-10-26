#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

void USARTx_Init(USART_TypeDef *USARTx, uint32_t BauRate);
void USART_Config_All(void);
void USARTx_SendData(USART_TypeDef * USARTx, uint16_t data);
#endif
