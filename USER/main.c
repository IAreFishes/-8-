/*******************************************************************************
* @file    main.c 
* @author  hc
* @version V1.0
* @date    2015/11/16
* @brief   none									
*******************************************************************************/
#include "includes.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];

int main(void)
{
	BSP_Init();
	OSInit();          
	OSTaskCreate(Task_Start,(void *)0,
        &startup_task_stk[STARTUP_TASK_STK_SIZE-1],STARTUP_TASK_PRIO);
	OSStart();
	return 0;
}


/******************* (C) COPYRIGHT 2015 *******END OF FILE********************/
