/*******************************************************************************
* @file    pwr.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

void Pwr_Ctrl(PWRER_MODE_TypeDef mode)
{
//	PWR_DeInit();
//	PWR_WakeUpPinCmd(ENABLE);    //ʹ�����Ż���
//	switch(mode)
//	{
//        POWER_NORMAL:break;
//		POWER_SLEEP: __WFI();break;   	                                          //����˯��ģʽ
//		POWER_STOP: PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //����ֹͣģʽ
//		POWER_STANDBY: PWR_EnterSTANDBYMode();break;     		 //�������ģʽ
//		default :break;
//	}
}
