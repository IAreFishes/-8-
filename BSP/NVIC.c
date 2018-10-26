/*******************************************************************************
* @file    NVIC.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  中断向量表配置
  * @param  void
  * @retval None
  */
void NVIC_Configuration_All(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级组2 
	
//	NVIC_Configuration(USART1_IRQn, 3,3);
//	NVIC_Configuration(USART2_IRQn, 3,3);
	NVIC_Configuration(USART3_IRQn, 2, 3);
//	NVIC_Configuration(UART4_IRQn, 3,3);
//	NVIC_Configuration(UART5_IRQn, 3,3);
	
	NVIC_Configuration(USB_LP_CAN1_RX0_IRQn, 0,0);
//	NVIC_Configuration(CAN1_RX1_IRQn, 0,3);
//	NVIC_Configuration(CAN1_SCE_IRQn, 1,1);

//	NVIC_Configuration(EXTI0_IRQn, 1,3);
//	NVIC_Configuration(EXTI1_IRQn, 1,3);
//	NVIC_Configuration(EXTI2_IRQn, 1,3);
//	NVIC_Configuration(EXTI3_IRQn, 1,3);
//	NVIC_Configuration(EXTI4_IRQn, 1,3);
//	NVIC_Configuration(EXTI9_5_IRQn, 1,3);
	NVIC_Configuration(EXTI15_10_IRQn, 0,3);
	
	NVIC_Configuration(TIM2_IRQn, 1,0);
	NVIC_Configuration(TIM3_IRQn, 1,1);
	NVIC_Configuration(TIM4_IRQn, 1,2);
//	NVIC_Configuration(TIM5_IRQn, 1,3);
//	NVIC_Configuration(TIM6_IRQn, 1,3);

//	NVIC_Configuration(RTC_IRQn, 1,3);

//  NVIC_Configuration(DMA1_Channel1_IRQn, 0, 4);
//	NVIC_Configuration(DMA1_Channel2_IRQn, 1, 1);
//	NVIC_Configuration(DMA1_Channel3_IRQn, 1, 2);
//	NVIC_Configuration(DMA1_Channel4_IRQn, 1, 3);
//	NVIC_Configuration(DMA1_Channel5_IRQn, 0, 1);
//	NVIC_Configuration(DMA1_Channel6_IRQn, 0, 2);
//	NVIC_Configuration(DMA1_Channel7_IRQn, 0, 3);
}

void NVIC_Configuration(uint8_t IRQChannel,uint8_t PreemptionPriority, uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}
