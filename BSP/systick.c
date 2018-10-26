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
  * @brief  SysTick����ΪAHB��8��Ƶ �� 72/8=9MHZ
  * @param  void
  * @retval Ϊ���ܹ���ucos-II���ܹ�ʹ��sistick ʵ��us������ʱ
  */
void Systick_Configuration(void)
{
//	#if SYSTICK_UCOSII == 0
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //ѡ���ⲿʱ�� 72MHZ/8=9MHZ
	fac_us=SystemCoreClock/8000000; //9��������Ϊ 1us
	fac_ms=fac_us*1000;
//	#endif 
}

/**
  * @brief  us����ʱ
  * @param  nus 
  * @retval ��ʱ��ʱ�򲻻ᱻ����ϵͳ������
  */
void delay_us(u32 nus)
{
	#if SYSTICK_UCOSII == 0  //���ò���ϵͳʱ��
		u32 temp;
		
		SysTick->LOAD = nus*fac_us;
		SysTick->VAL = 0;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //�򿪶�ʱ��
		do
		{
			temp = SysTick->CTRL;
		}while(temp&0x01&&!(temp&(1<<16)));  //�ȴ�ʱ�䵽��
		SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk; //�رն�ʱ��
		SysTick->VAL = 0;
	#endif
		
	#if SYSTICK_UCOSII == 1  //ʹ�ò���ϵͳʱ��
		u32 ticks, told, tnow, tcnt=0;
		u32 reload = SysTick->LOAD;
		ticks = nus*fac_us;
		tcnt = 0;
		OSSchedLock();  //�ر����������
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
		OSSchedUnlock(); //�������������
		
	#endif
}

/**
  * @brief  �������Ķ�ʱ�����ж�
  * @param  void
  * @retval None
  */
void Systick_Start(void)
{
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

