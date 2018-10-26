/*******************************************************************************
* @file    app.c 
* @author  hc
* @version V1.0
* @date    2015/7/17
* @brief   Ӧ�ó����ļ�									
*******************************************************************************/
#include "includes.h"
OS_STK Display_task_stk[DISPLAY_TASK_STK_SIZE];	
OS_STK DAQ_task_stk[DAQ_TASK_STK_SIZE];	
OS_STK WhiteBalance_task_stk[WhiteBalance_TASK_STK_SIZE];
OS_STK Bounded_Check_task_stk[Bounded_Check_TASK_STK_SIZE];	
OS_STK USART_task_stk[USART_TASK_STK_SIZE];
OS_STK Site_Scanning_task_stk[Site_Scanning_TASK_STK_SIZE];
OS_STK Key_Scan_task_stk[Key_Scan_TASK_STK_SIZE];
OS_STK Count_Ehite_Line_task_stk[Count_Ehite_Line_TASK_STK_SIZE];

OS_EVENT *TCS230_Semp;//TCS230���ݲɼ�����ź���
OS_EVENT *KeyScan_Semp;//����ɨ���������
OS_EVENT *TaskEnd_Semp;//������������ź���
/**
  * @brief  ����Task_Start ��������  
  * @param  void *p_arg
  * @retval None
  */ 
INT8U err;  // �������ʹ�ŵı���
void Task_Start(void *p_arg)
{
	
	INT32U cpu_sr;   //�����ٽ����Ҫ����˾ֲ�����
	(void)p_arg;//��û���õ���Ϊ��ֹ��������ʾ����
    
	OS_ENTER_CRITICAL();  //�����ٽ��
    /*************����������************/
    OSTaskCreate(Task_Display,(void *)0,
        &Display_task_stk[DISPLAY_TASK_STK_SIZE-1],DISPLAY_TASK_PRIO);
    OSTaskCreate(Task_DAQ,(void *)0,
        &DAQ_task_stk[DAQ_TASK_STK_SIZE-1],DAQ_TASK_PRIO);
    OSTaskCreate(WhiteBalance_task,(void *)0,
        &WhiteBalance_task_stk[WhiteBalance_TASK_STK_SIZE-1],WhiteBalance_TASK_PRIO);
    OSTaskCreate(Task_Bounded_Check,(void *)0,
        &Bounded_Check_task_stk[Bounded_Check_TASK_STK_SIZE-1],Bounded_Check_TASK_PRIO);
    OSTaskCreate(Task_USART,(void *)0,
        &USART_task_stk[USART_TASK_STK_SIZE-1],USART_TASK_PRIO);
    OSTaskCreate(Task_Site_Scanning,(void *)0,
        &Site_Scanning_task_stk[Site_Scanning_TASK_STK_SIZE-1],Site_Scanning_TASK_PRIO);
    OSTaskCreate(Task_Key_Scan,(void *)0,
        &Key_Scan_task_stk[Key_Scan_TASK_STK_SIZE-1],Key_Scan_TASK_PRIO);
    OSTaskCreate(Task_Count_Ehite_Line,(void *)0,
        &Count_Ehite_Line_task_stk[Count_Ehite_Line_TASK_STK_SIZE-1],Count_Ehite_Line_TASK_PRIO);
    
    TCS230_Semp = OSSemCreate(1);  //�����ź���
    KeyScan_Semp = OSSemCreate(1); //�����ź���
    TaskEnd_Semp = OSSemCreate(1); //�����ź���
	OS_EXIT_CRITICAL();   //�˳��ٽ��
	for(;;)
	{
        OSTaskSuspend(STARTUP_TASK_PRIO);
		OSTimeDlyHMSM(0,0,0,100);
	}	
}

/**
  * @brief  ������ʾ����
  * @param  void *p_arg
  * @retval 
  */
void Task_Display(void *p_arg)
{
	 float t;
	
	(void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
//    OSTaskSuspend(DISPLAY_TASK_PRIO);
    for(;;)
    {
//        OSSemPend(TaskEnd_Semp,0,&err); //�����ź���
        LCD_Display_int(R_Val[0],0,0);	
		LCD_Display_int(G_Val[0],6,0);
        LCD_Display_int(B_Val[0],12,0);
       
		LCD_Display_int(R_Val[1],0,1);	
		LCD_Display_int(G_Val[1],6,1);
        LCD_Display_int(B_Val[1],12,1);
     
		LCD_Display_int(R_Val[2],0,2);	
		LCD_Display_int(G_Val[2],6,2);
        LCD_Display_int(B_Val[2],12,2);

		LCD_Display_int(R_Val[3],0,3);	
		LCD_Display_int(G_Val[3],6,3);
        LCD_Display_int(B_Val[3],12,3);

		LCD_Display_int(R_Val[4],0,4);	
		LCD_Display_int(G_Val[4],6,4);
        LCD_Display_int(B_Val[4],12,4);

		LCD_Display_int(R_Val[5],0,5);	
		LCD_Display_int(G_Val[5],6,5);
        LCD_Display_int(B_Val[5],12,5);

		LCD_Display_int(R_Val[6],0,6);	
		LCD_Display_int(G_Val[6],6,6);
        LCD_Display_int(B_Val[6],12,6);

		LCD_Display_int(R_Val[7],0,7);	
		LCD_Display_int(G_Val[7],6,7);
		t=(float)R_Val[7]/(float)B_Val[7];
        LCD_Display_float(t,12,7);
        
        
        LCD_Display_int(colour_flag_num,18,2);
        LCD_Display_int(colour_flag,18,4);
        LCD_Display_int(white_line_num,18,5);       
        LCD_Display_int(adjust_direction,18,6);  
        LCD_Display_int(adjust_distance,18,7);
         
        OSTimeDlyHMSM(0,0,0,200);
    }
}
/**
  * @brief  ����TCS230��ƽ������
  * @param  void *p_arg
  * @retval None
  */
void WhiteBalance_task(void *p_arg)
{
	(void)p_arg;//��û���õ���Ϊ��ֹ��������ʾ����
	TCS230_LED(1);//�رղ����
	OSTimeDlyHMSM(0,0,0,500);
    TIM_Cmd(TIM2,ENABLE);  
    TIM_Cmd(TIM3,ENABLE);  
    TIM_Cmd(TIM4,ENABLE);    
    OSTaskSuspend(WhiteBalance_TASK_PRIO);
	for(;;)
	{	
        TCS230_Error_LED(0);//���źŵ�
		White_Balance(1000);	  //��ƽ��	
        TCS230_Error_LED(1);    
        OSTaskSuspend(WhiteBalance_TASK_PRIO);        
	}
    
}

/**
  * @brief  �������ݲɼ�����
  * @param  void *p_arg
  * @retval ��8�� ���벶��ͨ�� �õ� VDATA_NUM �� ����
            ɨ���־λ������ԭʼ���ݽ���ת�����˲�
            �õ�������������
  */
void Task_DAQ(void *p_arg)
{
	(void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
    TCS230_CAP_Task();
}


/**
  * @brief  ������ɫ�߽��ж�����
  * @param  void *p_arg
  * @retval ��CAN1����ÿ�μ���жϵĽ��  ���ƫ�� ƫ����ֵ �����Ҹ�
  */
void Task_Bounded_Check(void *p_arg)
{
	(void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
	for(;;)
	{
//        OSSemPend(TCS230_Semp,0,&err); //�����ź���
        adjust_distance = Judge_Colour();
		
		  	printf("distance:%d\n\r", adjust_distance);
//   	   CAN1_SendData(LIGHT_BOARD_ID, (uint16_t)adjust_distance);       
		OSTimeDlyHMSM(0,0,0,1);
	}	
}

/**
  * @brief  �����������ݴ�������
  * @param  void *p_arg
  * @retval None
  */
void Task_USART(void *p_arg)
{
    int n = 0;
    (void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
    OSTaskSuspend(USART_TASK_PRIO);
    for(;;)
    {
        	
        for(n=0;n<50;n++)
        {
//            for(i=0;i<8;i++)
//            {    
//                printf("%d\t",R_Val[3]);
//      
//                printf("%d\t",G_Val[i]);

//                printf("%d\t",B_Val[i]);
//                printf("\n");  
//            }
//            printf("\n");   
//            printf("\n");
            OSTimeDlyHMSM(0,0,0,5);              
        }   
        TCS230_Error_LED(1); 
        OSTaskSuspend(USART_TASK_PRIO);
        OSTimeDlyHMSM(0,0,0,5);        
    }
}

/**
  * @brief  ��������ɨ��,��������CANָ���
  * @param  void *p_arg
  * @retval None
  */

void Task_Site_Scanning(void *p_arg)
{
    (void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
    OSTaskSuspend(Site_Scanning_TASK_PRIO);	
    for(;;)
    {
        TCS230_Error_LED(0);//���źŵ�   
			
        Site_Scanning(colour_flag_num);  //���������ı궨���Զ�colour_flag_num��1
        TCS230_Error_LED(1);
        OSTaskSuspend(Site_Scanning_TASK_PRIO); 
    }
    
}

/**
  * @brief  ����ɨ������
  * @param  void *p_arg
  * @retval None
  */
void Task_Key_Scan(void *p_arg)
{
    (void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
    OSTaskSuspend(Key_Scan_TASK_PRIO);
    for(;;)
    {
        Key_Scan();  
        OSTaskSuspend(Key_Scan_TASK_PRIO);
    }
}

/**
  * @brief  ��������������
  * @param  void *p_arg
  * @retval 
  */
void Task_Count_Ehite_Line(void *p_arg)
{
    (void)p_arg;               // 'p_arg' ��û���õ�����ֹ��������ʾ����
    OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);
	for(;;)
	{
//        OSSemPend(TCS230_Semp,0,&err); //�����ź���
		Count_white_Line();
        CAN1_SendData(LIGHT_BOARD_ID, adjust_distance=100);   //���͹̶�ƫ��100 ��ʶΪ����ģʽ
        OSTimeDlyHMSM(0,0,0,5);  
//        OSSemPost(TaskEnd_Semp);       //��������� 
    }
}
