/*******************************************************************************
* @file    adc.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

uint16_t ADC1Value[ADC1_COUNT];

/**
  * @brief  ADC初始化配置，DMA方式
  * @param  void
  * @retval None
  */
void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

	/* ADCCLK = PCLK2/6 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置ADC时钟源 APB2的4分频72/6=12MHZ 最大不超过14MHZ
	/* Enable ADC1 clocks */   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	     //打开ADC1的时钟

  /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;         //独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						   //多通道扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;				 //工作在连续
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		 //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC1_COUNT;					//开启通道数					 
    ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5);  //ADC1采样的通道和采样时间设置
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles5);  //28.5个周期的采样时间
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_28Cycles5); 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
//  采样时间计算为  T= 循环周期28.5+12.5；

	/****************************内部温度传感器通道16************************/
	//V25=1.43v  Avg_Slope=0.0043
	//误差范围 +- 1.5 度
  //使能ADC1模块DMA
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
//	ADC_TempSensorVrefintCmd(ENABLE);   //使能内部温度传感器通道
//  LCD_DisNum(90, 100, ((1.43-(float)ADC1Value[0]/4095.0*3.3)/0.0043+25), YELLOW); // CPU温度显示
	/****************************内部温度传感器通道16************************/
	
  //使能ADC1模块DMA
  ADC_DMACmd(ADC1, ENABLE); 
    //打开ADC1
  ADC_Cmd(ADC1, ENABLE);

 //重置ADC1校准寄存器 
  ADC_ResetCalibration(ADC1); 
 //等待ADC1校准重置完成
  while(ADC_GetResetCalibrationStatus(ADC1));  

  //开始ADC1校准
  ADC_StartCalibration(ADC1); 

 //等待ADC1校准完成
  while(ADC_GetCalibrationStatus(ADC1)); 

 //使能ADC1软件开始转换
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
}

/**
  * @brief  DMA配置
  * @param  void
  * @retval None
  */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* Enable the DMA1 and DMA2 Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

  //配置ADC1的DMA配置
  DMA_DeInit(DMA1_Channel1);          //复位DMA1通道一设置
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		 //定义外设寄存器基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1Value;		 //定义DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						 //外设作为数据传输的来源 
  DMA_InitStructure.DMA_BufferSize = ADC1_COUNT;							   //缓存大小以DMA_PeripheralDataSize或DMA_MemoryDataSize为数据单位
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					 //接收一次数据后，目标内存地址后移
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //外设数据宽度为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							   //转换模式，循环缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;						 //DMA优先级高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								   //没有设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);                    //使能通道1
}
/**
  * @brief  ADC1初始化配置，DMA方式
  * @param  void
  * @retval None
  */
void ADC1_Init(void)
{
	ADC_Config();
	DMA_Configuration();
}
