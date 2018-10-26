#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include "includes.h"

/***控制器通信结构体***/
typedef struct
{
	unsigned Servo_ID:7;
	unsigned Data_Len:7;
	unsigned Data_Format:2;
	unsigned Data_Type:2;
	uint8_t  Front_Data[4];
	int32_t  Last_Data;
}Can_Servo;

void Servo_Set_Function(uint8_t Len,unsigned ID,uint8_t Command,int32_t  Data);
void Angle_Mode_Config(uint8_t ID);
void Speed_Mode_Config(uint8_t ID);
void Send_Speed_Acc(uint8_t ID,int32_t speed, int16_t acc);
#endif

/****************************电机命令参数表******************************/
/*
uint8_t Len,unsigned ID,uint8_t Command,int32_t  Data

Servo_Set_Function(8, ID, 0x2a, 0+Signal_Source+Set_date_Mode*4096+Set_date*65536);   //设定电机工作于速度模式
Servo_Set_Function(8, ID, 0x2a, 256+Signal_Source+Set_date_Mode*4096+Set_date*65536); //设定电机工作于角度模式

Servo_Set_Function(8, ID, 0x98, (4000*angle*16)/360);       //设定当前位置为电机的绝对零点位置
Servo_Set_Function(8, ID, 0x98, (2000*angle*16)/360);       //RE25 设定当前位置为电机的绝对零点位置

Servo_Set_Function(8, ID, 0x9a, (4000*angle*16)/360);	    //设定电机转动的相对转动角度
Servo_Set_Function(8, ID, 0x9a, (2000*angle*16)/360);		//RE25 设定电机转动的相对转动角度

Servo_Set_Function(8, ID, 0x99, (4000*angle*16)/360);		//设定电机转动的绝对转动角度
Servo_Set_Function(8, ID, 0x99, (2000*angle*16)/360);		//RE25 设定电机转动的绝对转动角度

Servo_Set_Function(8, ID, 0x1e, code*4);				    //设置编码器的分辨率

Servo_Set_Function(8, ID, 0x90, speed*16);					//设定电机的速度
Servo_Set_Function(8, ID, 0x16, max_speed*16);				//设定电机的最大速度	
Servo_Set_Function(8, ID, 0x18, min_speed*16);			    //设定电机的最小速度
Servo_Set_Function(6, ID, 0x58, Set_acc);      			    //设定电机的加速度
Servo_Set_Function(8, ID, 0x33, (4000*angle*16)/360);		//设置电机正向位置上限
Servo_Set_Function(8, ID, 0x35, (4000*angle*16)/360);	    //设置电机反向位置上限
Servo_Set_Function(6, ID, 0x3F, ON/OFF);					//设置和取消软件限位
Servo_Set_Function(6, ID, 0x41, ON/OFF);					//设置和取消硬件限位
Servo_Set_Function(8, ID, 0x1A, mA);                        //设定峰值电流 0~40000mA
Servo_Set_Function(8, ID, 0x1C, mA);                        //设定电机连续电流值 0~20000mA
Servo_Set_Function(6, ID, 0x31, mV); 		     			//设定死区电压

Servo_Set_Function(4, ID, 0x9b, 0);							//读取电机的实时位置
Servo_Set_Function(4, ID, 0x9F, 0);                         //读取电机的实时误差
Servo_Set_Function(4, ID, 0x91, 0);							//读取电机的平均速度

Servo_Set_Function(6, ID, 0x66, P_Date);                    //设置位置环比例系数
Servo_Set_Function(6, ID, 0x6A, I_Date);                    //设置位置环积分系数
Servo_Set_Function(6, ID, 0x6C, D_Date);                    //设置位置环积分系数

Servo_Set_Function(6, ID, 0x60, P_Date);                    //设置速度环比例系数
Servo_Set_Function(6, ID, 0x62, I_Date);                    //设置速度环积分系数
Servo_Set_Function(6, ID, 0x64, D_Date);                    //设置速度环积分系数

Servo_Set_Function(6, ID, 0X5A, P_Date);                    //设置电流环比例系数
Servo_Set_Function(6, ID, 0x5C, I_Date);                    //设置电流环积分系数
Servo_Set_Function(6, ID, 0x5E, D_Date);                    //设置电流环积分系数

Servo_Set_Function(4, ID, 0x80, 0);						    //设定驱动器加载电机
Servo_Set_Function(4, ID, 0x81, 0);					        //设定驱动器释放电机电机
Servo_Set_Function(4, ID, 0x82, 0);					        //保存设置的参数入电机驱动器的EEPROM
Servo_Set_Function(4, ID, 0x84, 0);						    //软件急停
Servo_Set_Function(4, ID, 0x85, 0);							//解除软件急停
Servo_Set_Function(6, ID, 0x04, set_ID);                    //设定电机的ID
Servo_Set_Function(8, ID, 0x07, baudrate);					//设置波特率
Servo_Set_Function(4, ID, 0xD2, 0);							//读取故障信息

Servo_Set_Function(6, ID, 0x75, 1);				            //开启寻找零位
Servo_Set_Function(8, ID, 0x77, speed*16);				    //设定寻找零位速度
Servo_Set_Function(8, ID, 0x79, point);				        //寻找零位最大范围

*/
