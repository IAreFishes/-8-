#ifndef __FOUR_WHEEL_CHASSIS_H
#define __FOUR_WHEEL_CHASSIS_H
#include "includes.h"

#define Pi 3.1415926

/***���ֿ�������ID***/      //��ʱ�����Ϸ����ӿ�ʼΪ ��һ��
#define  Servo_ID_1     10  
#define	 Servo_ID_2	    17  
#define  Servo_ID_3  	11  
#define  Servo_ID_4  	16  

#define CLASSIS_L2      0.3    //���̿�ȣ���λm��
#define CLASSIS_L1      0.2    //���̳��ȣ���λm��

void Motor_Init_config(void);
void Walk_XY_Vmod(int32_t Vx,int32_t Vy,int16_t Wz,int16_t  Acc);
#endif
