/*******************************************************************************
* @file    usart.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  串口初始化配置
  * @param  void
  * @retval None
  */	
void USART_Config_All(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF_PP);   // 配置PB10为TX
    GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_IN_FLOATING); //配置 PB11为RX
    USARTx_Init(USART3, 115200);
    
	
	
}
void USARTx_Init(USART_TypeDef *USARTx, uint32_t BauRate)
{
    USART_InitTypeDef USART_InitStructure;
    
    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = BauRate;  //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //一帧数据长度
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No ;  //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);

    USART_ClearITPendingBit(USARTx, USART_IT_RXNE);  //清除接收中断标志位
    USART_ClearITPendingBit(USARTx, USART_IT_TC);    //清除发送中断标志位
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  //打开接收数据寄存器非空中断

    //发送完成标志位中断,千万不能初始化中打开发送完成中断，否则会立刻引发一次中断，为什么呢？
    //USART_ITConfig(USARTx, USART_IT_TC, ENABLE);	  
}



void USARTx_SendData(USART_TypeDef * USARTx, uint16_t data)
{
	USART_SendData(USARTx, data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)== RESET);
}

/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(USART3, (uint8_t) ch);
    
    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		

    return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
    /* 等待串口1输入数据 */
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART3);
}
