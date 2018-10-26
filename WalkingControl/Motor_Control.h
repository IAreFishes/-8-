#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#include "includes.h"

/***������ͨ�Žṹ��***/
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

/****************************������������******************************/
/*
uint8_t Len,unsigned ID,uint8_t Command,int32_t  Data

Servo_Set_Function(8, ID, 0x2a, 0+Signal_Source+Set_date_Mode*4096+Set_date*65536);   //�趨����������ٶ�ģʽ
Servo_Set_Function(8, ID, 0x2a, 256+Signal_Source+Set_date_Mode*4096+Set_date*65536); //�趨��������ڽǶ�ģʽ

Servo_Set_Function(8, ID, 0x98, (4000*angle*16)/360);       //�趨��ǰλ��Ϊ����ľ������λ��
Servo_Set_Function(8, ID, 0x98, (2000*angle*16)/360);       //RE25 �趨��ǰλ��Ϊ����ľ������λ��

Servo_Set_Function(8, ID, 0x9a, (4000*angle*16)/360);	    //�趨���ת�������ת���Ƕ�
Servo_Set_Function(8, ID, 0x9a, (2000*angle*16)/360);		//RE25 �趨���ת�������ת���Ƕ�

Servo_Set_Function(8, ID, 0x99, (4000*angle*16)/360);		//�趨���ת���ľ���ת���Ƕ�
Servo_Set_Function(8, ID, 0x99, (2000*angle*16)/360);		//RE25 �趨���ת���ľ���ת���Ƕ�

Servo_Set_Function(8, ID, 0x1e, code*4);				    //���ñ������ķֱ���

Servo_Set_Function(8, ID, 0x90, speed*16);					//�趨������ٶ�
Servo_Set_Function(8, ID, 0x16, max_speed*16);				//�趨���������ٶ�	
Servo_Set_Function(8, ID, 0x18, min_speed*16);			    //�趨�������С�ٶ�
Servo_Set_Function(6, ID, 0x58, Set_acc);      			    //�趨����ļ��ٶ�
Servo_Set_Function(8, ID, 0x33, (4000*angle*16)/360);		//���õ������λ������
Servo_Set_Function(8, ID, 0x35, (4000*angle*16)/360);	    //���õ������λ������
Servo_Set_Function(6, ID, 0x3F, ON/OFF);					//���ú�ȡ�������λ
Servo_Set_Function(6, ID, 0x41, ON/OFF);					//���ú�ȡ��Ӳ����λ
Servo_Set_Function(8, ID, 0x1A, mA);                        //�趨��ֵ���� 0~40000mA
Servo_Set_Function(8, ID, 0x1C, mA);                        //�趨�����������ֵ 0~20000mA
Servo_Set_Function(6, ID, 0x31, mV); 		     			//�趨������ѹ

Servo_Set_Function(4, ID, 0x9b, 0);							//��ȡ�����ʵʱλ��
Servo_Set_Function(4, ID, 0x9F, 0);                         //��ȡ�����ʵʱ���
Servo_Set_Function(4, ID, 0x91, 0);							//��ȡ�����ƽ���ٶ�

Servo_Set_Function(6, ID, 0x66, P_Date);                    //����λ�û�����ϵ��
Servo_Set_Function(6, ID, 0x6A, I_Date);                    //����λ�û�����ϵ��
Servo_Set_Function(6, ID, 0x6C, D_Date);                    //����λ�û�����ϵ��

Servo_Set_Function(6, ID, 0x60, P_Date);                    //�����ٶȻ�����ϵ��
Servo_Set_Function(6, ID, 0x62, I_Date);                    //�����ٶȻ�����ϵ��
Servo_Set_Function(6, ID, 0x64, D_Date);                    //�����ٶȻ�����ϵ��

Servo_Set_Function(6, ID, 0X5A, P_Date);                    //���õ���������ϵ��
Servo_Set_Function(6, ID, 0x5C, I_Date);                    //���õ���������ϵ��
Servo_Set_Function(6, ID, 0x5E, D_Date);                    //���õ���������ϵ��

Servo_Set_Function(4, ID, 0x80, 0);						    //�趨���������ص��
Servo_Set_Function(4, ID, 0x81, 0);					        //�趨�������ͷŵ�����
Servo_Set_Function(4, ID, 0x82, 0);					        //�������õĲ���������������EEPROM
Servo_Set_Function(4, ID, 0x84, 0);						    //�����ͣ
Servo_Set_Function(4, ID, 0x85, 0);							//��������ͣ
Servo_Set_Function(6, ID, 0x04, set_ID);                    //�趨�����ID
Servo_Set_Function(8, ID, 0x07, baudrate);					//���ò�����
Servo_Set_Function(4, ID, 0xD2, 0);							//��ȡ������Ϣ

Servo_Set_Function(6, ID, 0x75, 1);				            //����Ѱ����λ
Servo_Set_Function(8, ID, 0x77, speed*16);				    //�趨Ѱ����λ�ٶ�
Servo_Set_Function(8, ID, 0x79, point);				        //Ѱ����λ���Χ

*/
