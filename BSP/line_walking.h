#ifndef __LINE_WALKING_H
#define __LINE_WALKING_H
#include "includes.h"
/*ÏÞ·ù²ÎÊý*/
#define Robot_Omiga_Max  250
#define Robot_Omiga_min  40
#define LEDBoard_ID  5 

void  Direction_Control_PD(signed int P, signed int D, unsigned int Acc);
void Receive_LightBoard_Data(void);
int32_t ABS(int32_t num);
#endif
