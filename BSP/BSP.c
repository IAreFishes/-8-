/*******************************************************************************
* @file    BSP.c 
* @author  hc
* @version V1.0
* @date    2015/8/25
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  所有外设在这里初始化
  * @param  void
  * @retval None
  */
void BSP_Init(void)
{
	SystemInit();	       	 //配置系统时钟为72MHZ 3.5的库版本中可以不用 系统默认初始化为72M
	RCC_Configuration();     //初始化RCC
	SysTick_init();      	 //初始化并时能SysTick定时器
	NVIC_Configuration_All();//中断向量表配置
//	GPIO_Config_All();    	  //所有GPIO初始化
    USART_Config_All();
    TIMX_Config_All();
//	ADC1_Init();          	//ADC1配置
//	RTC_Configuration();    //RTC配置
//	IWDG_Configuration();   //独立看门狗配置
	CAN1_Configuration();   //CAN1配置
	EXTI_AllConfiguration();//所有外部中断配置
	LCD_Init();             //lcd12864初始化
	TCS230_Config();        //TCS230初始化
} 

/**
  * @brief  SysTick初始化
  * @param  void
  * @retval None
  */
void SysTick_init(void)
{
	//初始化并使能SysTick定时器  72M/1000=72000 次计数产生1ms中断
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC); 
																											
}
