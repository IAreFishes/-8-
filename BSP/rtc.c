/*******************************************************************************
* @file    rtc.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  rtc��ʼ��
  * @param  void
  * @retval None
  */

void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */  // �򿪵�Դ�ͱ��ݼĴ�����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */ // ���ݼĴ���д��������
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */        // ��λ���ݼĴ���
	BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);       // ���ⲿ����ʱ��
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) // �ȴ�LSE����
	{}
	/* Select LSE as RTC Clock Source */  // ����LSEΪRTCʱ��Դ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);	
	/* Wait for RTC registers synchronization 
	 * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ�� */
	RTC_WaitForSynchro();	 
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	  // ÿ�ζԼĴ�����������Ҫ�ȴ�д��������
	/* Enable the RTC Second */
		
//	RTC_ITConfig(RTC_IT_SEC, ENABLE);	 // �������ж�
		
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
  /* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */ //����Ϊ1S
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	  // ÿ�ζԼĴ�����������Ҫ�ȴ�д��������	
		
}
