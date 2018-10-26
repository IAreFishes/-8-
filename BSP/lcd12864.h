#ifndef __LCD12864_H
#define __LCD12864_H

#include "includes.h"

#define CS(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define RES(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define RS(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_14);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define SCLK(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_13);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define SID(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_15);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_15)


void LCD_GPIO_Config(void);
void LCD_Init(void);
void Send_Command(unsigned char command);
void Send_data(unsigned char data);
void Clear_LCD(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_write_char(char c);
void LCD_Write_string(char *table,unsigned char X, unsigned char Y);
void LCD_Display_float(float Data,unsigned char X, unsigned char Y);
void LCD_Display_int(signed int Data,unsigned char X, unsigned char Y);
void float2str(float data, char*s);
void int2str(signed int data, char*s);					

					
#endif					
					
					
	


