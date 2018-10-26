/*******************************************************************************
* @file    usart.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  ���ڳ�ʼ������
  * @param  void
  * @retval None
  */	
void USART_Config_All(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF_PP);   // ����PB10ΪTX
    GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_IN_FLOATING); //���� PB11ΪRX
    USARTx_Init(USART3, 115200);
    
	
	
}
void USARTx_Init(USART_TypeDef *USARTx, uint32_t BauRate)
{
    USART_InitTypeDef USART_InitStructure;
    
    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = BauRate;  //����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //һ֡���ݳ���
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No ;  //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);

    USART_ClearITPendingBit(USARTx, USART_IT_RXNE);  //��������жϱ�־λ
    USART_ClearITPendingBit(USARTx, USART_IT_TC);    //��������жϱ�־λ
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  //�򿪽������ݼĴ����ǿ��ж�

    //������ɱ�־λ�ж�,ǧ���ܳ�ʼ���д򿪷�������жϣ��������������һ���жϣ�Ϊʲô�أ�
    //USART_ITConfig(USARTx, USART_IT_TC, ENABLE);	  
}



void USARTx_SendData(USART_TypeDef * USARTx, uint16_t data)
{
	USART_SendData(USARTx, data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)== RESET);
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ�USART1 */
    USART_SendData(USART3, (uint8_t) ch);
    
    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		

    return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
    /* �ȴ�����1�������� */
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART3);
}
