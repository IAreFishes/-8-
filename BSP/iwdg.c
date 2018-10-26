/*******************************************************************************
* @file    iwdg.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief  �������Ź���ʼ��
  * @param  void
  * @retval ���Ź�ι������Ϊ  Ԥ��Ƶֵ/40KHZ*��װ��ֵ
  */
//���� ~~������ ���Ź�������û������
void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(KR_KEY_ACCESS);   //ʹ�ܷ��ʶ������Ź��Ĵ���
	IWDG_SetPrescaler(IWDG_Prescaler_256);//����Ԥ��ƵֵΪ256
	while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET); //�ȴ�Ԥ��Ƶ����
	IWDG_SetReload(400);            //������װ��ֵΪ400  ���12λ4095
	while(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET);  //�ȴ���ת��ֵ����
	IWDG_ReloadCounter();           // ι��
	RCC_LSICmd(ENABLE);              //��LSI �����ڲ�����
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);// �ȴ�LSI�ȶ�
	IWDG_Enable();                  //ʹ�ܶ������Ź�	
}
