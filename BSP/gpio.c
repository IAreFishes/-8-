/*******************************************************************************
* @file    gpio.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  ����gpio��ʼ��
  * @param  void
  * @retval None
  */
void GPIO_Config_All(void)
{

}

/**
  * @brief  gpio��ʼ��
  * @param  void
  * @retval None
  */
void GPIO_Config(GPIO_TypeDef *GPIOx, uint16_t PINX, GPIOMode_TypeDef GPIO_MODE)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA<<((int)(GPIOx-GPIOA_BASE)/0x0400), ENABLE);
     GPIO_InitStructure.GPIO_Pin = PINX;
     GPIO_InitStructure.GPIO_Mode = GPIO_MODE;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOx, &GPIO_InitStructure);		
}

/**************************************************************************
*����GPIO��������
*void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)    ��λ
*void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)  ���λ
*uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)  ��λ
**************************************************************************/
