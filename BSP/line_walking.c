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
  * @brief  巡线PD闭环控制
  * @param  P参数   D参数   Acc加速度
  * @retval 只旋转角度
  */    
void  Direction_Control_PD(signed int P, signed int D, unsigned int Acc)
{
    signed int Robot_Omiga = 0;
    Robot_Omiga = P*(lightboard_ek_now+D*lightboard_ek_change);  //通过PD参数计算旋转控制量
    if(Robot_Omiga>Robot_Omiga_Max)       Robot_Omiga=Robot_Omiga_Max;
	if(Robot_Omiga<(-Robot_Omiga_Max))    Robot_Omiga=Robot_Omiga_Max;
	if(ABS(Robot_Omiga)<Robot_Omiga_min)  Robot_Omiga=0;
    Walk_XY_Vmod(0, 150,Robot_Omiga ,Acc);
}

/**
  * @brief  CAN1接受灯板数据处理
  * @param  
  * @retval 
  */  
void Receive_LightBoard_Data(void)
{
    CanRxMsg RxMessage;
	RxMessage.StdId = 0x00;         //初始化清零
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.DLC = 0;
	RxMessage.Data[0] = 0x00;
	RxMessage.Data[1] = 0x00;       //初始化清零
	RxMessage.Data[2] = 0x00;
	CAN_Receive(CAN1, CAN_FIFO0,&RxMessage);
    if(RxMessage.StdId==LEDBoard_ID&&RxMessage.Data[2]==LEDBoard_ID)   //接收灯板数据
	{
        if(RxMessage.Data[1]==1)   //纠偏方向向左，偏差输出为负值
        {
            lightboard_ek_change = lightboard_ek_now;
            lightboard_ek_now=-RxMessage.Data[0];
            lightboard_ek_change=lightboard_ek_now-lightboard_ek_change;    //得到与上一次偏差的差值
        }
        else            //纠偏方向向右，偏差输出为正值
        {
            lightboard_ek_change = lightboard_ek_now;
            lightboard_ek_now = RxMessage.Data[0];
            lightboard_ek_change=lightboard_ek_now-lightboard_ek_change;    //得到与上一次偏差的差值
        } 
	}
	else if( (RxMessage.StdId==1)&&(RxMessage.Data[3]==0x80)&&(RxMessage.Data[2]==0xd2) )  //电机数据
	{
		  //电机接收数据代码
	}
}

/*******************************************************************************
* 函数名：ABS
* 描述  ：取绝对值
* 输入  ：num  ：待取绝对值的数
* 输出  ：无
* 返回  ：num的绝对值
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
