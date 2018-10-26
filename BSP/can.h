#ifndef __CAN_H
#define __CAN_H
#include "includes.h"

#define GPIO_CAN 		   GPIOB     //PB     PD      PA  
#define GPIO_Pin_CAN_RX    GPIO_Pin_8   //PIN8   PIN0    PIN11 
#define GPIO_Pin_CAN_TX    GPIO_Pin_9   //PIN9   PIN1    PIN12
#define GPIO_Remap_CAN     GPIO_Remap1_CAN1  //GPIO_Remap2_CAN1  GPIO_Remap1_CAN1
#define CAN_BAUDRATE 500      //…Ë÷√CAN≤®Ãÿ¬ 
void CAN1_Configuration(void); 
#endif
