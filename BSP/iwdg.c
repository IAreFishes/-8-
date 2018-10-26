/*******************************************************************************
* @file    iwdg.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  独立看门狗初始化
  * @param  void
  * @retval 看门狗喂狗周期为  预分频值/40KHZ*重装载值
  */
//问题 ~~！！！ 看门狗的周期没法设置
void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(KR_KEY_ACCESS);   //使能访问独立看门狗寄存器
	IWDG_SetPrescaler(IWDG_Prescaler_256);//设置预分频值为256
	while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET); //等待预分频更新
	IWDG_SetReload(400);            //设置重装载值为400  最大12位4095
	while(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET);  //等待重转载值更新
	IWDG_ReloadCounter();           // 喂狗
	RCC_LSICmd(ENABLE);              //打开LSI 低速内部晶振
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);// 等待LSI稳定
	IWDG_Enable();                  //使能独立看门狗	
}
