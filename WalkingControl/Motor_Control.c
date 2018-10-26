#include "includes.h"

CanRxMsg RxMessage; //can������Ϣȫ�ֱ���
/**
  * @brief  Can��ս�������
  * @param  CanRxMsg *RxMessage ��������ṹ 
  * @retval None
  */
void Can_ClearRxMessage(CanRxMsg *RxMessage)
{
	uint8_t i = 0;
	
	RxMessage->StdId = 0x00;			/* ��׼��ʶ�� */
	RxMessage->ExtId = 0x00;			/* ��չ��ʶ�� */
	RxMessage->IDE = CAN_ID_STD;	    /* ��չ��ʶ��ʹ�� */
	RxMessage->DLC = 0;				    /* �������ݳ��� */
	RxMessage->FMI = 0;				    /* ������ƥ����� */
	for (i=0; i<8; i++)					/* �������������� */
	{
		RxMessage->Data[i] = 0;
	}
		
}

/**
  * @brief  Can ���߸�������������Ϣ����
  * @param  Can_Servo *servo
  * @retval None
  */
uint8_t Servo_SendMessage(Can_Servo *servo)
{
	uint8_t TransmitMailbox;
	CanTxMsg TxMessage;
	int i = 0;
	TxMessage.StdId = servo->Servo_ID;         //ID ����
	TxMessage.IDE = servo->Data_Format;		   //���ݸ�ʽ
	TxMessage.RTR = servo->Data_Type;		   //��������
 	TxMessage.DLC = servo->Data_Len;		   //���ݳ��� ��λΪ�ֽ�
	for(;i<4;i++)
	{
		TxMessage.Data[i] = servo->Front_Data[i];
	}
	for(i=0;i<4;i++)
	{
		TxMessage.Data[7-i] = (((servo->Last_Data)>>(24-8*i))&0xff);   // ���ݴӵ�λ��ʼ����
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
	return  0;     //���ͳɹ�
}	

/**
  * @brief  ���ŷ�����������CANָ��
  * @param  ��ϸ�����趨�뿴ͷ�ļ� "Motor_Control.h" β��
  * @retval None
  */
void Servo_Set_Function(uint8_t Len,unsigned ID,uint8_t Command,int32_t  Data)
{
	Can_Servo servo;
	
	servo.Data_Len=Len;
	servo.Servo_ID=ID;
	servo.Last_Data=Data;					  
	servo.Data_Format = CAN_ID_STD; 		//��׼֡
	servo.Data_Type   = CAN_RTR_DATA;		//����֡
	servo.Front_Data[0]=servo.Data_Len;	    //�������ݳ���
	servo.Front_Data[1] =servo.Servo_ID;    //ָ�����Ƶ�Ŀ������ID
	servo.Front_Data[2] = Command;			//���������CANָ��
	servo.Front_Data[3] = 0;
	Servo_SendMessage(&servo);
}

/**
  * @brief  ��������óɽǶ�ģʽ
  * @param  ID ������ID��
  * @retval None
  */
void Angle_Mode_Config(uint8_t ID)
{
	Servo_Set_Function(4, ID, 0x81, 0);					//�趨�������ͷŵ�����
	Servo_Set_Function(8, ID, 0x98, (4000*0*16)/360);   //�趨��ǰλ��Ϊ����ľ������λ��
	Servo_Set_Function(6, ID, 0x66, 5016);				//����λ�û�����ϵ�� ����ֵ
	Servo_Set_Function(8, ID, 0x16, 100*16);	    	//�趨���������ٶ�
	Servo_Set_Function(8, ID, 0x18, 50*16);				//�趨�������С�ٶ�
	Servo_Set_Function(8, ID, 0x1A, 10000);             //�趨��ֵ���� 0~40000mA
	Servo_Set_Function(8, ID, 0x1C, 5000);              //�趨�����������ֵ 0~20000mA
	Servo_Set_Function(8, ID, 0x1e, 500*4);			    //���ñ������ķֱ���
	Servo_Set_Function(8, ID, 0x2a, 256);               //�趨��������ڽǶ�ģʽ
	Servo_Set_Function(6, ID, 0x58, 100); 				//�趨����ļ��ٶ�
	Servo_Set_Function(4, ID, 0x82, 0);					//�������õĲ���������������EEPROM
	OSTimeDlyHMSM(0,0,0,10);
	Servo_Set_Function(4, ID, 0x80, 0);			    	 //�趨���������ص��
}

/**
  * @brief  ��������ó��ٶ�ģʽ
  * @param  ID ������ID��
  * @retval None
  */
void Speed_Mode_Config(uint8_t ID)
{
	Servo_Set_Function(4, ID, 0x81, 0);					//�趨�������ͷŵ�����
	Servo_Set_Function(8, ID, 0x16, 300*16);			//�趨���������ٶ�
	Servo_Set_Function(8, ID, 0x18, 50*16);				//�趨�������С�ٶ�	
	Servo_Set_Function(8, ID, 0x1A, 10000);             //�趨��ֵ���� 0~40000mA
	Servo_Set_Function(8, ID, 0x1C, 5000);              //�趨�����������ֵ 0~20000mA
	Servo_Set_Function(8, ID, 0x1e, 500*4);			    //���ñ������ķֱ���
	Servo_Set_Function(8, ID, 0x2a, 0);                 //�趨����������ٶ�ģʽ
	Send_Speed_Acc(ID, 0, 80);
	Servo_Set_Function(4, ID, 0x82, 0);					 //�������õĲ���������������EEPROM
	OSTimeDlyHMSM(0,0,0,10);
	Servo_Set_Function(4, ID, 0x80, 0);					 //�趨���������ص��
}
/**
  * @brief  �������������ٶȺͼ��ٶ���Ϣ
  * @param  ID ������ID��  speed�ٶ�  acc���ٶ�
  * @retval None
  */
void Send_Speed_Acc(uint8_t ID,int32_t speed, int16_t acc)
{
	Servo_Set_Function(6, ID, 0x58, acc);       //�趨����ļ��ٶ�
	OSTimeDlyHMSM(0,0,0,5);
	Servo_Set_Function(8, ID, 0x90, speed*16);	//�趨������ٶ�
}
