#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

typedef enum
{
    TIME_MIN=1,
	TIME_US,
	TIME_MS,
	TIME_S,
}TIME_Unit_TypeDef;

typedef enum
{
    TIM_Cap_IT=1,   //中断模式
    TIM_Cap_DMA     //DMA模式
}TIME_Cap_Mode;

void TIMX_Config_All(void);
void TIMx_Configuration(TIM_TypeDef *TIMx, TIME_Unit_TypeDef time_unit, uint16_t times); 
void TIMX_Capture_All(void);
void TIMx_Capture_Config(void);
void TIMX_PWM_Out_All(void);
void TIMx_PWMOut_Config(TIM_TypeDef *TIMx,uint16_t Pulse1, uint16_t Pulse2, uint16_t Pulse3, uint16_t Pulse4);
#endif
