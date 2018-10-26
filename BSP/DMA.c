#include "includes.h"
/**
  * @brief  DMA_Config初始化
  * @param  DR_Add 数据来源地址 MMB_Add 传送目标地址 
			BufferSize 数据大小（单位16位）Channel DMA 的通道
  * @retval 使用DMA时需要打开外设和DMA的通道，
            TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);
            打开DMA和定时器的通道TIM_DMACmd
            还要使能DMA通道DMA_Cmd(DMA1_Channel5,ENABLE);
  */
void DMA1_Config(uint32_t DR_Add,uint32_t MMB_Add,uint32_t BufferSize,DMA_Channel_TypeDef *Channel)
{
    DMA_InitTypeDef DMA_InitStructure;    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
    DMA_DeInit(Channel);   
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_Add;   			//源数据地址  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)MMB_Add; //目标内存地址	
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        //数据传送方向
    DMA_InitStructure.DMA_BufferSize = BufferSize;    	      //数据大小（单位为DMA_PeripheralDataSize）
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  	      //内存数据为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            //循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;        //优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  						 //内存到内存传输关闭
    DMA_Init(Channel,&DMA_InitStructure); 	                   //配置通道
	DMA_ITConfig(Channel, DMA_IT_TC, ENABLE);				   //打开通道传输完成中断
	
}
