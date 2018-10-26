/*******************************************************************************
* @file    rtc.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  rtc初始化
  * @param  void
  * @retval None
  */

void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */  // 打开电源和备份寄存器的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* Allow access to BKP Domain */ // 备份寄存器写操作请求
	PWR_BackupAccessCmd(ENABLE);
	/* Reset Backup Domain */        // 复位备份寄存器
	BKP_DeInit();
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);       // 打开外部低速时钟
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) // 等待LSE起振
	{}
	/* Select LSE as RTC Clock Source */  // 设置LSE为RTC时钟源
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);	
	/* Wait for RTC registers synchronization 
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步 */
	RTC_WaitForSynchro();	 
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	  // 每次对寄存器操作都需要等待写操作结束
	/* Enable the RTC Second */
		
//	RTC_ITConfig(RTC_IT_SEC, ENABLE);	 // 设置秒中断
		
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
  /* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */ //周期为1S
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	  // 每次对寄存器操作都需要等待写操作结束	
		
}
