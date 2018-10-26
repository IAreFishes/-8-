/*******************************************************************************
* @file    can.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"


/**
  * @brief  CAN总线配置
  * @param  void
  * @retval None
  */

void CAN1_Configuration(void)
{  
	CAN_InitTypeDef  CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_Config(GPIO_CAN, GPIO_Pin_CAN_RX, GPIO_Mode_IPU);    // 上拉输入
	GPIO_Config(GPIO_CAN, GPIO_Pin_CAN_TX, GPIO_Mode_AF_PP); // 推挽输出
	
	GPIO_PinRemapConfig(GPIO_Remap_CAN, ENABLE);           // 重映射
	/*************************************************
	                  CAN 总线配置
	*************************************************/
	CAN_DeInit(CAN1);  //复位
	CAN_StructInit(&CAN_InitStructure);	//缺省设置
	
	CAN_InitStructure.CAN_TTCM = DISABLE;  // 设置时间触发通信模式（失能）启用产生时间戳
	CAN_InitStructure.CAN_ABOM = ENABLE;   // 使/失能自动离线管理（失能） 当错误累计到一定时进入离线状态
	CAN_InitStructure.CAN_AWUM = DISABLE;  // 使/失能自动唤醒模式（失能） 由软件唤醒
	CAN_InitStructure.CAN_NART = ENABLE;  // 使/失能非自动重传输模式（失能）一直重传直到发送成功
	CAN_InitStructure.CAN_RFLM = DISABLE;  // 使/失能接收FIFO锁定模式（失能）接受的报文满邮箱后自动覆盖
	CAN_InitStructure.CAN_TXFP = ENABLE;   // 使/失能发送FIFO优先级（失能）优先级由发送请求顺序决定
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常模式
	
	/* 下面定义波特率预分频器 */
	/* 波特率 = 1 / ((1 + BS1 + BS2) x tq)，tq取自PCLK1的分频 */
	/*75%     when 波特率 > 800K
		80%     when 波特率 > 500K
		87.5%   when 波特率 <= 500K */
		//分频系数要进过计算
#if CAN_BAUDRATE == 1000 /* 1MBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 6;
#elif CAN_BAUDRATE == 800 /* 800KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;         // 时段1位5个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         // 时段2位3个时间单元 定义发送点的位置	
	CAN_InitStructure.CAN_Prescaler = 5;
#elif CAN_BAUDRATE == 500 /* 500KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 12;
#elif CAN_BAUDRATE == 400 /* 400KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;         // 时段1位5个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;         // 时段2位3个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 10;
#elif  CAN_BAUDRATE == 250 /* 250KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 24;
#elif  CAN_BAUDRATE == 200 /* 200KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 30;
#elif  CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 60;
#elif  CAN_BAUDRATE == 80 /* 80KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 75;
#elif  CAN_BAUDRATE == 62.5 /* 62.5KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 96;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 120;
#elif  CAN_BAUDRATE == 40 /* 40KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 150;
	#elif  CAN_BAUDRATE == 25 /* 25KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 240;
	#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 300;
	#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;         // 同步跳跃1个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;         // 时段1位3个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;         // 时段2位2个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 600;
	#elif  CAN_BAUDRATE == 5 /* 5KBps */
  CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;         // 同步跳跃2个时间单元
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;         // 时段1位6个时间单元 定义采样点的位置
	CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;         // 时段2位4个时间单元 定义发送点的位置
	CAN_InitStructure.CAN_Prescaler = 600;
#else
	#error "Please select first the CAN Baudrate in Private defines in CAN.c "
#endif  /* CAN_BAUDRATE == 1000 */
	CAN_Init(CAN1, &CAN_InitStructure);
	
	/**************************************************
	              CAN 总线过滤器设置
	**************************************************/
	CAN_FilterInitStructure.CAN_FilterNumber = 0;          //指定初始化过滤器编号  （0~13）        
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;  //标识符屏蔽位模式、过滤一组ID
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //1个32位的过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0;     //设定过滤器的标识符 （32位为其高段，16位为第一个）范围（0x0000~0xffff）
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;      //设定过滤器的标识符  （32位为其低段，16位为第二个）范围（0x0000~0xffff）
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0; //设定过滤器屏蔽标识符或者过滤标识符,为1的位必须与标识符严格一致
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;  //设定过滤器屏蔽标识符或者过滤标识符，为1的位必须与标识符严格一致
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //过滤器FIPO指向第0组
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;     //使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
    /* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);  //FIFO0 消息挂号中断屏蔽
//	CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);  //FIFO1 消息挂号中断屏蔽

//	CAN_ITConfig(CAN1,CAN_IT_BOF, ENABLE);	/* Bus-off 离线中断*/ 
//	CAN_ITConfig(CAN1,CAN_IT_EWG, ENABLE);	/* Error warning 错误警告中断*/
//	CAN_ITConfig(CAN1,CAN_IT_EPV, ENABLE);	/* Error passive 错误被动中断*/	
}
