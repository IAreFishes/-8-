#include "includes.h"
/**
  * @brief  DMA_Config��ʼ��
  * @param  DR_Add ������Դ��ַ MMB_Add ����Ŀ���ַ 
			BufferSize ���ݴ�С����λ16λ��Channel DMA ��ͨ��
  * @retval ʹ��DMAʱ��Ҫ�������DMA��ͨ����
            TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);
            ��DMA�Ͷ�ʱ����ͨ��TIM_DMACmd
            ��Ҫʹ��DMAͨ��DMA_Cmd(DMA1_Channel5,ENABLE);
  */
void DMA1_Config(uint32_t DR_Add,uint32_t MMB_Add,uint32_t BufferSize,DMA_Channel_TypeDef *Channel)
{
    DMA_InitTypeDef DMA_InitStructure;    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
    DMA_DeInit(Channel);   
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_Add;   			//Դ���ݵ�ַ  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)MMB_Add; //Ŀ���ڴ��ַ	
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        //���ݴ��ͷ���
    DMA_InitStructure.DMA_BufferSize = BufferSize;    	      //���ݴ�С����λΪDMA_PeripheralDataSize��
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  	      //�ڴ�����Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            //ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;        //���ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  						 //�ڴ浽�ڴ洫��ر�
    DMA_Init(Channel,&DMA_InitStructure); 	                   //����ͨ��
	DMA_ITConfig(Channel, DMA_IT_TC, ENABLE);				   //��ͨ����������ж�
	
}
