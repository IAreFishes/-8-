/*******************************************************************************
* @file    BSP.c 
* @author  hc
* @version V1.0
* @date    2015/8/25
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  ���������������ʼ��
  * @param  void
  * @retval None
  */
void BSP_Init(void)
{
	SystemInit();	       	 //����ϵͳʱ��Ϊ72MHZ 3.5�Ŀ�汾�п��Բ��� ϵͳĬ�ϳ�ʼ��Ϊ72M
	RCC_Configuration();     //��ʼ��RCC
	SysTick_init();      	 //��ʼ����ʱ��SysTick��ʱ��
	NVIC_Configuration_All();//�ж�����������
//	GPIO_Config_All();    	  //����GPIO��ʼ��
    USART_Config_All();
    TIMX_Config_All();
//	ADC1_Init();          	//ADC1����
//	RTC_Configuration();    //RTC����
//	IWDG_Configuration();   //�������Ź�����
	CAN1_Configuration();   //CAN1����
	EXTI_AllConfiguration();//�����ⲿ�ж�����
	LCD_Init();             //lcd12864��ʼ��
	TCS230_Config();        //TCS230��ʼ��
} 

/**
  * @brief  SysTick��ʼ��
  * @param  void
  * @retval None
  */
void SysTick_init(void)
{
	//��ʼ����ʹ��SysTick��ʱ��  72M/1000=72000 �μ�������1ms�ж�
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC); 
																											
}
