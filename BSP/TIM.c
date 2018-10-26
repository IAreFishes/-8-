/*******************************************************************************
* @file    TIM.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  定时器基时定时配置
  * @param  void
  * @retval None
  */
void TIMX_Config_All(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIMx_Configuration(TIM2, TIME_MIN, 0xffff); 
    TIMx_Configuration(TIM3, TIME_MIN, 0xffff);
    TIMx_Configuration(TIM4, TIME_MIN, 0xffff);    
    TIMX_Capture_All();   //  捕获函数用模版中的形式会出问题  
}

/**
  * @brief  定时器基时定时配置
  * @param  void
  * @retval None
  */
void TIMx_Configuration(TIM_TypeDef *TIMx, TIME_Unit_TypeDef time_unit, uint16_t times)  
{
		
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
		TIM_TimeBaseStructure.TIM_Period=times-1; 
        switch(time_unit)	
		{
            case TIME_MIN:TIM_TimeBaseStructure.TIM_Prescaler=0;  //不分频
										TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; break;
			case TIME_US:TIM_TimeBaseStructure.TIM_Prescaler=71;  //中断的驱动时钟频率为72/(71+1)=1MHZ
			             TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; break;  // 单位为1us
			case TIME_MS:TIM_TimeBaseStructure.TIM_Prescaler=35999; //中断的驱动时钟频率为72/(35999+1)=2KHZ
									 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
									 TIM_TimeBaseStructure.TIM_Period=times*2-1;break;  // 单位为1Ms  最多32S
			case TIME_S:TIM_TimeBaseStructure.TIM_Prescaler=35999;  //中断的驱动时钟频率为72/(35999+1)=2KHZ
			             TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
                   TIM_TimeBaseStructure.TIM_Period=times*2000-1; break;   // 单位为1s  最多32S
			default :break;
		} 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIMx, TIM_FLAG_Update);
		TIM_Cmd(TIMx, ENABLE);	
}


/**
  * @brief  配置指定定时器为输入捕获功能
  * @param  void
  * @retval None
  */
void TIMX_Capture_All(void)
{
    //TIM2  CCR1 2 3
    GPIO_Config(GPIOA, GPIO_Pin_0, GPIO_Mode_IN_FLOATING);
    GPIO_Config(GPIOA, GPIO_Pin_1, GPIO_Mode_IN_FLOATING);
    GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Mode_IN_FLOATING);
    //TIM3 CCR1 2 3
    GPIO_Config(GPIOA, GPIO_Pin_6, GPIO_Mode_IN_FLOATING);
    GPIO_Config(GPIOA, GPIO_Pin_7, GPIO_Mode_IN_FLOATING);
    GPIO_Config(GPIOB, GPIO_Pin_0, GPIO_Mode_IN_FLOATING);
    //TIM4 CCR1 2
    GPIO_Config(GPIOB, GPIO_Pin_6, GPIO_Mode_IN_FLOATING);
    GPIO_Config(GPIOB, GPIO_Pin_7, GPIO_Mode_IN_FLOATING);
    //捕获模式配置  捕获中断模式
    TIMx_Capture_Config();
				
}
/**
  * @brief  TIMx 捕获模式配置
  * @param  void
  * @retval 如果使用DMA方式 还需要配置DMA通道 DMA1_Config 
            打开DMA_Cmd 通道才能使用
  */
void TIMx_Capture_Config(void)
{
		TIM_ICInitTypeDef TIM_ICInitStructure;
	
		TIM_DeInit(TIM2);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 输入捕获上升沿
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 引脚直接映射通道
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 捕获时每探测一个边沿执行一次
		TIM_ICInitStructure.TIM_ICFilter = 0;                         // 滤波设置经历几个周期确认波形稳定 0x00~0x0F
		TIM_ICInit(TIM2, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0;
		TIM_ICInit(TIM2, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0;
		TIM_ICInit(TIM2, &TIM_ICInitStructure);
//		
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
//		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//		TIM_ICInitStructure.TIM_ICFilter = 0x0;
//		TIM_ICInit(TIM2, &TIM_ICInitStructure);
    		
//		TIM_DMAConfig(TIM2, TIM_DMABase_CCR1,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM2, TIM_DMA_CC1, DISABLE);   
//		TIM_DMAConfig(TIM2, TIM_DMABase_CCR2,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM2, TIM_DMA_CC2, DISABLE);
//		TIM_DMAConfig(TIM2, TIM_DMABase_CCR3,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM2, TIM_DMA_CC3, DISABLE);
		
		/* TIM enable counter */
		TIM2->CNT=0;
		TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3, DISABLE);
		TIM_Cmd(TIM2,DISABLE);       // 关闭定时器	
		TIM_ClearFlag(TIM2, TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3);   
        
		TIM_DeInit(TIM3);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 输入捕获上升沿
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 引脚直接映射通道
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 捕获时每探测一个边沿执行一次
		TIM_ICInitStructure.TIM_ICFilter = 0;                         // 滤波设置经历几个周期确认波形稳定 0x00~0x0F
		TIM_ICInit(TIM3, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;      
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0;
		TIM_ICInit(TIM3, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0;
		TIM_ICInit(TIM3, &TIM_ICInitStructure);
//		
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
//		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//		TIM_ICInitStructure.TIM_ICFilter = 0;
//		TIM_ICInit(TIM3, &TIM_ICInitStructure);
//		
//		TIM_DMAConfig(TIM3, TIM_DMABase_CCR1,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM3, TIM_DMA_CC1, DISABLE);   
////		TIM_DMAConfig(TIM3, TIM_DMABase_CCR2,TIM_DMABurstLength_2Bytes);   
////    TIM_DMACmd(TIM3, TIM_DMA_CC2, DISABLE);  // 找不到这个DMA通道
//		TIM_DMAConfig(TIM3, TIM_DMABase_CCR3,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM3, TIM_DMA_CC3, DISABLE);
		
		/* TIM enable counter */
		TIM3->CNT=0;
		TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3, DISABLE);
		TIM_Cmd(TIM3,DISABLE);       // 关闭定时器
		TIM_ClearFlag(TIM3, TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3);
        
		TIM_DeInit(TIM4);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 输入捕获上升沿
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 引脚直接映射通道
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 捕获时每探测一个边沿执行一次
		TIM_ICInitStructure.TIM_ICFilter = 0x0;                         // 滤波设置经历几个周期确认波形稳定 0x00~0x0F
		TIM_ICInit(TIM4, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM4, &TIM_ICInitStructure);

//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
//		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//		TIM_ICInitStructure.TIM_ICFilter = 0x0;
//		TIM_ICInit(TIM4, &TIM_ICInitStructure);
//		
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
//		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//		TIM_ICInitStructure.TIM_ICFilter = 0x0;
//		TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
//		TIM_DMAConfig(TIM4, TIM_DMABase_CCR1,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM4, TIM_DMA_CC1, DISABLE);   
//		TIM_DMAConfig(TIM4, TIM_DMABase_CCR2,TIM_DMABurstLength_2Bytes);   
//		TIM_DMACmd(TIM4, TIM_DMA_CC2, DISABLE);
		
		/* TIM enable counter */
		TIM4->CNT=0;
		TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2, DISABLE);
		TIM_Cmd(TIM4,DISABLE);       // 关闭定时器	
		TIM_ClearFlag(TIM4, TIM_FLAG_CC1 | TIM_FLAG_CC2);
}

void TIMX_PWM_Out_All(void)
{
    GPIO_Config(GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP);  //通道1
    GPIO_Config(GPIOA, GPIO_Pin_7, GPIO_Mode_AF_PP);  //通道2
    GPIO_Config(GPIOB, GPIO_Pin_0, GPIO_Mode_AF_PP);  //通道3
    GPIO_Config(GPIOB, GPIO_Pin_1, GPIO_Mode_AF_PP);  //通道4
    TIMx_PWMOut_Config(TIM3, 0, 0, 0, 0);
}


/**
  * @brief  TIMx PWM 输出
  * @param  void
  * @retval None
  */
void TIMx_PWMOut_Config(TIM_TypeDef *TIMx,uint16_t Pulse1, uint16_t Pulse2, uint16_t Pulse3, uint16_t Pulse4)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置为PWM波模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能	   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //高电平跳变
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = Pulse1; //跳变值
	TIM_OC1Init(TIMx,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Pulse2;    //跳变值
    TIM_OC2Init(TIMx,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Pulse3;       //跳变值
	TIM_OC3Init(TIMx,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Pulse4;       //跳变值
	TIM_OC4Init(TIMx,&TIM_OCInitStructure);
//	
	TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);  //使能预装载
	TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIMx, ENABLE);			 // 使能TIMx重载寄存器ARR
	TIM_Cmd(TIMx, ENABLE);
}

