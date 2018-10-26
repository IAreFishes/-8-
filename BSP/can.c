/*******************************************************************************
* @file    can.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"


/**
  * @brief  CAN��������
  * @param  void
  * @retval None
  */

void CAN1_Configuration(void)
{  
	CAN_InitTypeDef  CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_Config(GPIO_CAN, GPIO_Pin_CAN_RX, GPIO_Mode_IPU);    // ��������
	GPIO_Config(GPIO_CAN, GPIO_Pin_CAN_TX, GPIO_Mode_AF_PP); // �������
	
	GPIO_PinRemapConfig(GPIO_Remap_CAN, ENABLE);           // ��ӳ��
	/*************************************************
	                  CAN ��������
	*************************************************/
	CAN_DeInit(CAN1);  //��λ
	CAN_StructInit(&CAN_InitStructure);	//ȱʡ����
	
	CAN_InitStructure.CAN_TTCM = DISABLE;  // ����ʱ�䴥��ͨ��ģʽ��ʧ�ܣ����ò���ʱ���
	CAN_InitStructure.CAN_ABOM = ENABLE;   // ʹ/ʧ���Զ����߹���ʧ�ܣ� �������ۼƵ�һ��ʱ��������״̬
	CAN_InitStructure.CAN_AWUM = DISABLE;  // ʹ/ʧ���Զ�����ģʽ��ʧ�ܣ� ���������
	CAN_InitStructure.CAN_NART = ENABLE;  // ʹ/ʧ�ܷ��Զ��ش���ģʽ��ʧ�ܣ�һֱ�ش�ֱ�����ͳɹ�
	CAN_InitStructure.CAN_RFLM = DISABLE;  // ʹ/ʧ�ܽ���FIFO����ģʽ��ʧ�ܣ����ܵı�����������Զ�����
	CAN_InitStructure.CAN_TXFP = ENABLE;   // ʹ/ʧ�ܷ���FIFO���ȼ���ʧ�ܣ����ȼ��ɷ�������˳�����
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //����ģʽ
	
	/* ���涨�岨����Ԥ��Ƶ�� */
	/* ������ = 1 / ((1 + BS1 + BS2) x tq)��tqȡ��PCLK1�ķ�Ƶ */
	/*75%     when ������ > 800K
		80%     when ������ > 500K
		87.5%   when ������ <= 500K */
		//��Ƶϵ��Ҫ��������
#if CAN_BAUDRATE == 1000 /* 1MBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 6;
#elif CAN_BAUDRATE == 800 /* 800KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;         // ʱ��1λ5��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         // ʱ��2λ3��ʱ�䵥Ԫ ���巢�͵��λ��	
	CAN_InitStructure.CAN_Prescaler = 5;
#elif CAN_BAUDRATE == 500 /* 500KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 12;
#elif CAN_BAUDRATE == 400 /* 400KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;         // ʱ��1λ5��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         // ʱ��2λ3��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 10;
#elif  CAN_BAUDRATE == 250 /* 250KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 24;
#elif  CAN_BAUDRATE == 200 /* 200KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 30;
#elif  CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 60;
#elif  CAN_BAUDRATE == 80 /* 80KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 75;
#elif  CAN_BAUDRATE == 62.5 /* 62.5KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 96;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 120;
#elif  CAN_BAUDRATE == 40 /* 40KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 150;
	#elif  CAN_BAUDRATE == 25 /* 25KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 240;
	#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 300;
	#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // ͬ����Ծ1��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // ʱ��1λ3��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // ʱ��2λ2��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 600;
	#elif  CAN_BAUDRATE == 5 /* 5KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;         // ͬ����Ծ2��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;         // ʱ��1λ6��ʱ�䵥Ԫ ����������λ��
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;         // ʱ��2λ4��ʱ�䵥Ԫ ���巢�͵��λ��
	CAN_InitStructure.CAN_Prescaler = 600;
#else
	#error "Please select first the CAN Baudrate in Private defines in CAN.c "
#endif  /* CAN_BAUDRATE == 1000 */
	CAN_Init(CAN1, &CAN_InitStructure);
	
	/**************************************************
	              CAN ���߹���������
	**************************************************/
	CAN_FilterInitStructure.CAN_FilterNumber = 0;          //ָ����ʼ�����������  ��0~13��        
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  //��ʶ������λģʽ������һ��ID
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //1��32λ�Ĺ�����
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0;     //�趨�������ı�ʶ�� ��32λΪ��߶Σ�16λΪ��һ������Χ��0x0000~0xffff��
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;      //�趨�������ı�ʶ��  ��32λΪ��ͶΣ�16λΪ�ڶ�������Χ��0x0000~0xffff��
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0; //�趨���������α�ʶ�����߹��˱�ʶ��,Ϊ1��λ�������ʶ���ϸ�һ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;  //�趨���������α�ʶ�����߹��˱�ʶ����Ϊ1��λ�������ʶ���ϸ�һ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //������FIPOָ���0��
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;     //ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	
    /* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  //FIFO0 ��Ϣ�Һ��ж�����
//	CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);  //FIFO1 ��Ϣ�Һ��ж�����

//	CAN_ITConfig(CAN1,CAN_IT_BOF, ENABLE);	/* Bus-off �����ж�*/ 
//	CAN_ITConfig(CAN1,CAN_IT_EWG, ENABLE);	/* Error warning ���󾯸��ж�*/
//	CAN_ITConfig(CAN1,CAN_IT_EPV, ENABLE);	/* Error passive ���󱻶��ж�*/	
}
