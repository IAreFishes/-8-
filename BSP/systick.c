/*******************************************************************************
* @file    systick.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"
#define SYSTICK_UCOSII 1
u32 fac_us=0, fac_ms=0;

/**
  * @brief  SysTick定义为AHB的8分频 及 72/8=9MHZ
  * @param  void
  * @retval 为了能够在ucos-II中能够使用sistick 实现us级的延时
  */
void Systick_Configuration(void)
{
//	#if SYSTICK_UCOSII == 0
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //选择外部时钟 72MHZ/8=9MHZ
	fac_us=SystemCoreClock/8000000; //9个计数后为 1us
	fac_ms=fac_us*1000;
//	#endif 
}

/**
  * @brief  us级延时
  * @param  nus 
  * @retval 延时的时候不会被操作系统任务打断
  */
void delay_us(u32 nus)
{
	#if SYSTICK_UCOSII == 0  //不用操作系统时候
		u32 temp;
		
		SysTick->LOAD = nus*fac_us;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //打开定时器
		do
		{
			temp = SysTick->CTRL;
		}while(temp&0x01&&!(temp&(1<<16)));  //等待时间到来
		SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk; //关闭定时器
		SysTick->VAL = 0;
	#endif
		
	#if SYSTICK_UCOSII == 1  //使用操作系统时候
		u32 ticks, told, tnow, tcnt=0;
		u32 reload = SysTick->LOAD;
		ticks = nus*fac_us;
		tcnt = 0;
		OSSchedLock();  //关闭任务调度器
		told = SysTick->VAL;
		while(1)
		{
			tnow = SysTick->VAL;
			if(tnow != told)
			{
				if(tnow < told)
				{
					tcnt += told-tnow;
				}
				else
				{
					tcnt += reload-tnow+told;
				}
				told = tnow;
				if(tcnt >= ticks)
				{
					break;
				}
			}
		}
		OSSchedUnlock(); //开启任务调度器
		
	#endif
}

/**
  * @brief  开启节拍定时器和中断
  * @param  void
  * @retval None
  */
void Systick_Start(void)
{
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

