/*******************************************************************************
* @file    line_walking.c 
* @author  hc
* @version V1.0
* @date    2015/11/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

signed int lightboard_ek_now = 0;
signed int lightboard_ek_change = 0;
/**
  * @brief  Ѳ��PD�ջ�����
  * @param  P����   D����   Acc���ٶ�
  * @retval ֻ��ת�Ƕ�
  */    
void  Direction_Control_PD(signed int P, signed int D, unsigned int Acc)
{
    signed int Robot_Omiga = 0;
    Robot_Omiga = P*(lightboard_ek_now+D*lightboard_ek_change);  //ͨ��PD����������ת������
    if(Robot_Omiga>Robot_Omiga_Max)       Robot_Omiga=Robot_Omiga_Max;
	if(Robot_Omiga<(-Robot_Omiga_Max))    Robot_Omiga=Robot_Omiga_Max;
	if(ABS(Robot_Omiga)<Robot_Omiga_min)  Robot_Omiga=0;
    Walk_XY_Vmod(0, 150,Robot_Omiga ,Acc);
}

/**
  * @brief  CAN1���ܵư����ݴ���
  * @param  
  * @retval 
  */  
void Receive_LightBoard_Data(void)
{
    CanRxMsg RxMessage;
	RxMessage.StdId = 0x00;         //��ʼ������
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.DLC = 0;
	RxMessage.Data[0] = 0x00;
	RxMessage.Data[1] = 0x00;       //��ʼ������
	RxMessage.Data[2] = 0x00;
	CAN_Receive(CAN1, CAN_FIFO0,&RxMessage);
    if(RxMessage.StdId==LEDBoard_ID&&RxMessage.Data[2]==LEDBoard_ID)   //���յư�����
	{
        if(RxMessage.Data[1]==1)   //��ƫ��������ƫ�����Ϊ��ֵ
        {
            lightboard_ek_change = lightboard_ek_now;
            lightboard_ek_now=-RxMessage.Data[0];
            lightboard_ek_change=lightboard_ek_now-lightboard_ek_change;    //�õ�����һ��ƫ��Ĳ�ֵ
        }
        else            //��ƫ�������ң�ƫ�����Ϊ��ֵ
        {
            lightboard_ek_change = lightboard_ek_now;
            lightboard_ek_now = RxMessage.Data[0];
            lightboard_ek_change=lightboard_ek_now-lightboard_ek_change;    //�õ�����һ��ƫ��Ĳ�ֵ
        } 
	}
	else if( (RxMessage.StdId==1)&&(RxMessage.Data[3]==0x80)&&(RxMessage.Data[2]==0xd2) )  //�������
	{
		  //����������ݴ���
	}
}

/*******************************************************************************
* ��������ABS
* ����  ��ȡ����ֵ
* ����  ��num  ����ȡ����ֵ����
* ���  ����
* ����  ��num�ľ���ֵ
*******************************************************************************/
int32_t ABS(int32_t num)
{
  if(num > 0)
  {
    return num;
  }
  else
  {
    return -num;
  }	  
}
