#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)   //DMA����������ַ
#define ADC1_COUNT		    5					              	//��ͨ��ADCͨ����
extern uint16_t ADC1Value[ADC1_COUNT];

void ADC_Config(void);
void DMA_Configuration(void);
void ADC1_Init(void);
#endif
