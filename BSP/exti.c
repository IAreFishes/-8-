/*******************************************************************************
* @file    exti.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

void EXTI_AllConfiguration(void)
{
    EXTI_Configuration(GPIO_PortSourceGPIOA, GPIO_PinSource12, EXTI_Trigger_Rising, ENABLE);      // 还需要设置GPIO 和 NVIC
}

void EXTI_Configuration(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource, EXTITrigger_TypeDef EXTI_Trigger_X,FunctionalState state)
{
    EXTI_InitTypeDef EXTI_Initstructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	

    GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);
    EXTI_Initstructure.EXTI_Line=1<<GPIO_PinSource;
    EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_X;
    EXTI_Initstructure.EXTI_LineCmd=state;
    EXTI_Init(&EXTI_Initstructure);
}
