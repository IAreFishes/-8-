/*******************************************************************************
* @file    app_cfg.h 
* @author  hc
* @version V1.0
* @date    2015/7/17
* @brief   ���������������ȼ���ջ��С									
*******************************************************************************/
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/*********************�����������ȼ�************************/
#define	STARTUP_TASK_PRIO	4
#define WhiteBalance_TASK_PRIO 5
#define Key_Scan_TASK_PRIO 6
#define DAQ_TASK_PRIO   7
#define Count_Ehite_Line_TASK_PRIO   8
#define Site_Scanning_TASK_PRIO  9
#define Bounded_Check_TASK_PRIO  10
#define USART_TASK_PRIO  11
#define DISPLAY_TASK_PRIO   12
 


/*********************����ջ��С����λΪ OS_STK��************************/
#define STARTUP_TASK_STK_SIZE	 128
#define DISPLAY_TASK_STK_SIZE   128
#define DAQ_TASK_STK_SIZE       256
#define WhiteBalance_TASK_STK_SIZE 128
#define Bounded_Check_TASK_STK_SIZE 256
#define USART_TASK_STK_SIZE     128
#define Site_Scanning_TASK_STK_SIZE 256
#define Key_Scan_TASK_STK_SIZE  128
#define Count_Ehite_Line_TASK_STK_SIZE 128
#endif

