#ifndef __APP_H__
#define __APP_H__

extern OS_EVENT *TCS230_Semp;//TCS230数据采集完毕信号量
extern OS_EVENT *TaskEnd_Semp;//所有任务结束信号量
extern OS_EVENT *KeyScan_Semp;//按键扫描任务结束

/********************用户任务声明**********************************/
void Task_Start(void *p_arg);
void Task_Display(void *p_arg);
void Task_DAQ(void *p_arg);
void WhiteBalance_task(void *p_arg);
void Task_Bounded_Check(void *p_arg);
void Task_USART(void *p_arg);
void Task_Site_Scanning(void *p_arg);
void Task_Key_Scan(void *p_arg);
void Task_Count_Ehite_Line(void *p_arg);
#endif //__APP_H__


