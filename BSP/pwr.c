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
//	PWR_WakeUpPinCmd(ENABLE);    //使能引脚唤醒
//	switch(mode)
//	{
//        POWER_NORMAL:break;
//		POWER_SLEEP: __WFI();break;   	                                          //进入睡眠模式
//		POWER_STOP: PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //进入停止模式
//		POWER_STANDBY: PWR_EnterSTANDBYMode();break;     		 //进入待机模式
//		default :break;
//	}
}
