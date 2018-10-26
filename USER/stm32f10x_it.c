/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    OSIntEnter(); 
    OSTimeTick(); 
    OSIntExit(); 
}

//USART3中断
void USART3_IRQHandler(void)	
{
	char n;
	 
	OSIntEnter(); 
	/*****************发送完成中断**************/
//	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)               
//	{
//		USART_ClearITPendingBit(USART3, USART_IT_TC);	
//	}
	
		/*****************接收中断*****************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		n=USART_ReceiveData(USART3);
		switch(n)
		{
			case '0': OSTaskSuspend(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(0);break;  //停止所有任务 待命
            case '1': OSTaskResume(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(1);break;   //巡线找线  //开灯
            case '2': OSTaskSuspend(Bounded_Check_TASK_PRIO); OSTaskResume(Count_Ehite_Line_TASK_PRIO);break;  //数线
            case '3': OSTaskResume(WhiteBalance_TASK_PRIO); break; //恢复白平衡任务
            case '4': OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=0;break;
			      case '5':  OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=1;break;
						case '6':	 OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=2;break;
						case '7':  OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=3;break;
						case '8':	 OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=4;break;
						case '9':  OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=5;break;
							
            default : break;  
			 
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
	OSIntExit();
}

volatile char num[8] = {0};
volatile uint8_t TIM_CCR_Status = 0;
//定时器2中断
void TIM2_IRQHandler (void)   
{
	OSIntEnter(); 
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1)==SET)
	{
        TIM2->SR = (uint16_t)~TIM_IT_CC1;
		TCS230_Period[7][num[7]]=TIM2->CCR1;;
		num[7]++;
		if(num[7]==VDATA_NUM)
		{
            TIM2->DIER &= (uint16_t)~TIM_IT_CC1;
			num[7] = 0;
			TIM_CCR_Status|=(1<<7);
		}
	}
    if(TIM_GetITStatus(TIM2, TIM_IT_CC2)==SET)
	{
        TIM2->SR = (uint16_t)~TIM_IT_CC2;
		TCS230_Period[6][num[6]]=TIM2->CCR2;
		num[6]++;
		if(num[6]==VDATA_NUM)
		{
            TIM2->DIER &= (uint16_t)~TIM_IT_CC2;
			num[6] = 0;
			TIM_CCR_Status|=(1<<6);
		}
	}
    if(TIM_GetITStatus(TIM2, TIM_IT_CC3)==SET)
	{
        TIM2->SR = (uint16_t)~TIM_IT_CC3;
		TCS230_Period[5][num[5]]=TIM2->CCR3;
		num[5]++;
		if(num[5]==VDATA_NUM)
		{			
            TIM2->DIER &= (uint16_t)~TIM_IT_CC3;
			num[5] = 0;
			TIM_CCR_Status|=(1<<5);
		}
	}
	OSIntExit(); 
}

//定时器3中断
void TIM3_IRQHandler (void) 
{
	OSIntEnter(); 
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1)==SET)
	{
        TIM3->SR = (uint16_t)~TIM_IT_CC1;
		TCS230_Period[4][num[4]]=TIM3->CCR1;
		num[4]++;
		if(num[4]==VDATA_NUM)
		{
            TIM3->DIER &= (uint16_t)~TIM_IT_CC1;
			num[4] = 0;
			TIM_CCR_Status|=(1<<4);
		}
	}
    if(TIM_GetITStatus(TIM3, TIM_IT_CC2)==SET)
	{
        TIM3->SR = (uint16_t)~TIM_IT_CC2;
		TCS230_Period[3][num[3]]= TIM3->CCR2;
		num[3]++;
		if(num[3]==VDATA_NUM)
		{
            TIM3->DIER &= (uint16_t)~TIM_IT_CC2;
			num[3] = 0;
			TIM_CCR_Status|=(1<<3);
		}
	}
    if(TIM_GetITStatus(TIM3, TIM_IT_CC3)==SET)
	{
        TIM3->SR = (uint16_t)~TIM_IT_CC3;
		TCS230_Period[2][num[2]]=TIM3->CCR3;
		num[2]++;
		if(num[2]==VDATA_NUM)
		{	
            TIM3->DIER &= (uint16_t)~TIM_IT_CC3;
			num[2] = 0;
			TIM_CCR_Status|=(1<<2);
		}
	}
	OSIntExit(); 
}

//定时器4中断
void TIM4_IRQHandler (void)   
{
	OSIntEnter(); 
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1)==SET)
	{
        TIM4->SR = (uint16_t)~TIM_IT_CC1;
		TCS230_Period[1][num[1]]=TIM4->CCR1;
		num[1]++;
		if(num[1]==VDATA_NUM)
		{
            TIM4->DIER &= (uint16_t)~TIM_IT_CC1;
			num[1] = 0;
			TIM_CCR_Status|=(1<<1);
		}
	}
    if(TIM_GetITStatus(TIM4, TIM_IT_CC2)==SET)
	{	
        TIM4->SR = (uint16_t)~TIM_IT_CC2;
		TCS230_Period[0][num[0]]=TIM4->CCR2;
		num[0]++;
		if(num[0]==VDATA_NUM)
		{
            TIM4->DIER &= (uint16_t)~TIM_IT_CC2;
			num[0] = 0;
			TIM_CCR_Status|=(1<<0);
		}
	}
	OSIntExit(); 
}


////RTC秒中断
//void RTC_IRQHandler(void)
//{
//	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)  // 检查是否发生秒中断
//	  {
//	    /* Clear the RTC Second interrupt */
//	    RTC_ClearITPendingBit(RTC_IT_SEC);
//	
//	    /* Enable time update */
////	    TimeDisplay = 1;
//	
//	    /* Wait until last write operation on RTC registers has finished */
//	    RTC_WaitForLastTask();
//	  }
//}

//void EXTI0_IRQHandler(void)
//{
//		OSIntEnter();	
//		if(EXTI_GetFlagStatus(EXTI_Line0)!=RESET)
//		{			  
//					 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1)
//					 {
//							GPIO_ResetBits(GPIOC, GPIO_Pin_4);
//					 }	
//						else GPIO_SetBits(GPIOC, GPIO_Pin_4);
//        EXTI_ClearITPendingBit(EXTI_Line0);
//				
//		}
//   	OSIntExit();
//}

void EXTI15_10_IRQHandler(void)
{
		OSIntEnter();	
		if(EXTI_GetFlagStatus(EXTI_Line12)!=RESET)
		{			 
            TCS230_Error_LED(0); 
            OSTaskResume(Key_Scan_TASK_PRIO);
            EXTI_ClearITPendingBit(EXTI_Line12);
				
		}
   	OSIntExit();
}

////CAN1接受中断
//void CAN1_RX1_IRQHandler(void)
//{
//    OSIntEnter();	
//	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);
//	OSIntExit();
//}
//CAN1接受中断
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    OSIntEnter();	
    CAN1_ReceiveData();
	OSIntExit();
}
//CAN1错误中断
//void CAN1_SCE_IRQHandler(void)
//{
//	OSIntEnter();
//	if(CAN_GetITStatus(CAN1,CAN_IT_BOF ) != RESET)	 /* Bus-off */
//	{
//		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF );
//	}
//	if(CAN_GetITStatus(CAN1, CAN_IT_EWG ) != RESET)	 /* Error warning */
//	{	
//		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG );
//	}
//	if(CAN_GetITStatus(CAN1, CAN_IT_EPV) != RESET)  	 /* Error passive */
//	{
//		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV );
//	}	
//	OSIntExit();	
//}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
