/*******************************************************************************
* @file    four_wheel_chassis.c 
* @author  hc
* @version V1.0
* @date    2015/11/04
* @brief   ���ֵ�����������									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  4�ֵ��̵����ʼ��
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
  * @brief  ��X��Y��������
  * @param  void
  * @retval None
  */\
void Walk_XY_Vmod(int32_t Vx,int32_t Vy,int16_t Wz,int16_t  Acc)
{
    int32_t  V1,V2,V3,V4;
	//�ٶȷ���
	Wz *= (CLASSIS_L1+CLASSIS_L2);    
	V1=Vx-Vy+Wz;	   //���һ����Ϊ��ת���ٶ�Ӧȡ��
	V2=-Vx-Vy+Wz;
	V3=-Vx+Vy+Wz;   //���������Ϊ��ת���ٶ�Ӧȡ��
	V4=Vx+Vy+Wz;
    Send_Speed_Acc(Servo_ID_1,V1,Acc);
    Send_Speed_Acc(Servo_ID_2,V2,Acc);
    Send_Speed_Acc(Servo_ID_3,V3,Acc);
    Send_Speed_Acc(Servo_ID_4,V4,Acc);
}
