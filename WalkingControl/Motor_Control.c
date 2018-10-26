#include "includes.h"

CanRxMsg RxMessage; //can接受消息全局变量
/**
  * @brief  Can清空接受邮箱
  * @param  CanRxMsg *RxMessage 接收邮箱结构 
  * @retval None
  */
void Can_ClearRxMessage(CanRxMsg *RxMessage)
{
	uint8_t i = 0;
	
	RxMessage->StdId = 0x00;			/* 标准标识符 */
	RxMessage->ExtId = 0x00;			/* 扩展标识符 */
	RxMessage->IDE = CAN_ID_STD;	    /* 扩展标识符使能 */
	RxMessage->DLC = 0;				    /* 接收数据长度 */
	RxMessage->FMI = 0;				    /* 过滤器匹配序号 */
	for (i=0; i<8; i++)					/* 清空邮箱里的数据 */
	{
		RxMessage->Data[i] = 0;
	}
		
}

/**
  * @brief  Can 总线给控制器发送消息函数
  * @param  Can_Servo *servo
  * @retval None
  */
uint8_t Servo_SendMessage(Can_Servo *servo)
{
	uint8_t TransmitMailbox;
	CanTxMsg TxMessage;
	int i = 0;
	TxMessage.StdId = servo->Servo_ID;         //ID 设置
	TxMessage.IDE = servo->Data_Format;		   //数据格式
	TxMessage.RTR = servo->Data_Type;		   //数据类型
 	TxMessage.DLC = servo->Data_Len;		   //数据长度 单位为字节
	for(;i<4;i++)
	{
		TxMessage.Data[i] = servo->Front_Data[i];
	}
	for(i=0;i<4;i++)
	{
		TxMessage.Data[7-i] = (((servo->Last_Data)>>(24-8*i))&0xff);   // 数据从低位开始发送
	}
	i=0 ;
	while(TransmitMailbox =CAN_Transmit(CAN1,&TxMessage), ((TransmitMailbox==CAN_NO_MB)&&(i<5)) )
	{
		  OSTimeDlyHMSM(0,0,0,1);
		  i++ ;
	}

    while((CAN_TransmitStatus(CAN1,TransmitMailbox)	== CANTXPENDING) && (i < 3000))
	{
		i++ ;
	}
	return  0;     //发送成功
}	

/**
  * @brief  给伺服驱动器发送CAN指令
  * @param  详细参数设定请看头文件 "Motor_Control.h" 尾部
  * @retval None
  */
void Servo_Set_Function(uint8_t Len,unsigned ID,uint8_t Command,int32_t  Data)
{
	Can_Servo servo;
	
	servo.Data_Len=Len;
	servo.Servo_ID=ID;
	servo.Last_Data=Data;					  
	servo.Data_Format = CAN_ID_STD; 		//标准帧
	servo.Data_Type   = CAN_RTR_DATA;		//数据帧
	servo.Front_Data[0]=servo.Data_Len;	    //发送数据长度
	servo.Front_Data[1] =servo.Servo_ID;    //指定控制的目标电机的ID
	servo.Front_Data[2] = Command;			//电机控制器CAN指令
	servo.Front_Data[3] = 0;
	Servo_SendMessage(&servo);
}

/**
  * @brief  将电机配置成角度模式
  * @param  ID 驱动器ID号
  * @retval None
  */
void Angle_Mode_Config(uint8_t ID)
{
	Servo_Set_Function(4, ID, 0x81, 0);					//设定驱动器释放电机电机
	Servo_Set_Function(8, ID, 0x98, (4000*0*16)/360);   //设定当前位置为电机的绝对零点位置
	Servo_Set_Function(6, ID, 0x66, 5016);				//设置位置环比例系数 经验值
	Servo_Set_Function(8, ID, 0x16, 100*16);	    	//设定电机的最大速度
	Servo_Set_Function(8, ID, 0x18, 50*16);				//设定电机的最小速度
	Servo_Set_Function(8, ID, 0x1A, 10000);             //设定峰值电流 0~40000mA
	Servo_Set_Function(8, ID, 0x1C, 5000);              //设定电机连续电流值 0~20000mA
	Servo_Set_Function(8, ID, 0x1e, 500*4);			    //设置编码器的分辨率
	Servo_Set_Function(8, ID, 0x2a, 256);               //设定电机工作于角度模式
	Servo_Set_Function(6, ID, 0x58, 100); 				//设定电机的加速度
	Servo_Set_Function(4, ID, 0x82, 0);					//保存设置的参数入电机驱动器的EEPROM
	OSTimeDlyHMSM(0,0,0,10);
	Servo_Set_Function(4, ID, 0x80, 0);			    	 //设定驱动器加载电机
}

/**
  * @brief  将电机配置成速度模式
  * @param  ID 驱动器ID号
  * @retval None
  */
void Speed_Mode_Config(uint8_t ID)
{
	Servo_Set_Function(4, ID, 0x81, 0);					//设定驱动器释放电机电机
	Servo_Set_Function(8, ID, 0x16, 300*16);			//设定电机的最大速度
	Servo_Set_Function(8, ID, 0x18, 50*16);				//设定电机的最小速度	
	Servo_Set_Function(8, ID, 0x1A, 10000);             //设定峰值电流 0~40000mA
	Servo_Set_Function(8, ID, 0x1C, 5000);              //设定电机连续电流值 0~20000mA
	Servo_Set_Function(8, ID, 0x1e, 500*4);			    //设置编码器的分辨率
	Servo_Set_Function(8, ID, 0x2a, 0);                 //设定电机工作于速度模式
	Send_Speed_Acc(ID, 0, 80);
	Servo_Set_Function(4, ID, 0x82, 0);					 //保存设置的参数入电机驱动器的EEPROM
	OSTimeDlyHMSM(0,0,0,10);
	Servo_Set_Function(4, ID, 0x80, 0);					 //设定驱动器加载电机
}
/**
  * @brief  给控制器发送速度和加速度信息
  * @param  ID 驱动器ID号  speed速度  acc加速度
  * @retval None
  */
void Send_Speed_Acc(uint8_t ID,int32_t speed, int16_t acc)
{
	Servo_Set_Function(6, ID, 0x58, acc);       //设定电机的加速度
	OSTimeDlyHMSM(0,0,0,5);
	Servo_Set_Function(8, ID, 0x90, speed*16);	//设定电机的速度
}
