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
  * @brief  ADC��ʼ�����ã�DMA��ʽ
  * @param  void
  * @retval None
  */
void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;

	/* ADCCLK = PCLK2/6 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ����ADCʱ��Դ APB2��4��Ƶ72/6=12MHZ ��󲻳���14MHZ
	/* Enable ADC1 clocks */   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	     //��ADC1��ʱ��

  /* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;         //����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						   //��ͨ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;				 //����������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		 //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC1_COUNT;					//����ͨ����					 
    ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5);  //ADC1������ͨ���Ͳ���ʱ������
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles5);  //28.5�����ڵĲ���ʱ��
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_28Cycles5); 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
//  ����ʱ�����Ϊ  T= ѭ������28.5+12.5��

	/****************************�ڲ��¶ȴ�����ͨ��16************************/
	//V25=1.43v  Avg_Slope=0.0043
	//��Χ +- 1.5 ��
  //ʹ��ADC1ģ��DMA
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
//	ADC_TempSensorVrefintCmd(ENABLE);   //ʹ���ڲ��¶ȴ�����ͨ��
//  LCD_DisNum(90, 100, ((1.43-(float)ADC1Value[0]/4095.0*3.3)/0.0043+25), YELLOW); // CPU�¶���ʾ
	/****************************�ڲ��¶ȴ�����ͨ��16************************/
	
  //ʹ��ADC1ģ��DMA
  ADC_DMACmd(ADC1, ENABLE); 
    //��ADC1
  ADC_Cmd(ADC1, ENABLE);

 //����ADC1У׼�Ĵ��� 
  ADC_ResetCalibration(ADC1); 
 //�ȴ�ADC1У׼�������
  while(ADC_GetResetCalibrationStatus(ADC1));  

  //��ʼADC1У׼
  ADC_StartCalibration(ADC1); 

 //�ȴ�ADC1У׼���
  while(ADC_GetCalibrationStatus(ADC1)); 

 //ʹ��ADC1�����ʼת��
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
}

/**
  * @brief  DMA����
  * @param  void
  * @retval None
  */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* Enable the DMA1 and DMA2 Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);

  //����ADC1��DMA����
  DMA_DeInit(DMA1_Channel1);          //��λDMA1ͨ��һ����
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		 //��������Ĵ�������ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1Value;		 //����DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						 //������Ϊ���ݴ������Դ 
  DMA_InitStructure.DMA_BufferSize = ADC1_COUNT;							   //�����С��DMA_PeripheralDataSize��DMA_MemoryDataSizeΪ���ݵ�λ
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ������� 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					 //����һ�����ݺ�Ŀ���ڴ��ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�������ݿ��Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							   //ת��ģʽ��ѭ������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;						 //DMA���ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								   //û������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);                    //ʹ��ͨ��1
}
/**
  * @brief  ADC1��ʼ�����ã�DMA��ʽ
  * @param  void
  * @retval None
  */
void ADC1_Init(void)
{
	ADC_Config();
	DMA_Configuration();
}
