/*******************************************************************************
* @file    four_wheel_chassis.c 
* @author  hc
* @version V1.0
* @date    2015/11/04
* @brief   四轮底盘驱动程序									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  4轮底盘电机初始化
  * @param  void
  * @retval None
  */
void Motor_Init_config(void)
{
    Speed_Mode_Config(Servo_ID_1);
    Speed_Mode_Config(Servo_ID_2);
    Speed_Mode_Config(Servo_ID_3);
    Speed_Mode_Config(Servo_ID_4);
}

/**
  * @brief  向X，Y方向行走
  * @param  void
  * @retval None
  */\
void Walk_XY_Vmod(int32_t Vx,int32_t Vy,int16_t Wz,int16_t  Acc)
{
    int32_t  V1,V2,V3,V4;
	//速度分配
	Wz *= (CLASSIS_L1+CLASSIS_L2);    
	V1=Vx-Vy+Wz;	   //电机一正数为反转，速度应取反
	V2=-Vx-Vy+Wz;
	V3=-Vx+Vy+Wz;   //电机三正数为反转，速度应取反
	V4=Vx+Vy+Wz;
    Send_Speed_Acc(Servo_ID_1,V1,Acc);
    Send_Speed_Acc(Servo_ID_2,V2,Acc);
    Send_Speed_Acc(Servo_ID_3,V3,Acc);
    Send_Speed_Acc(Servo_ID_4,V4,Acc);
}
