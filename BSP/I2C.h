#ifndef __I2C_H
#define __I2C_H
#include "includes.h"
#define I2C_GPIO1 GPIOB
#define I2C_PIN_SCL1 GPIO_Pin_8
#define I2C_PIN_SDA1 GPIO_Pin_9
#define Num1_SDA(x)		             if(x==0)  GPIO_ResetBits(I2C_GPIO1,I2C_PIN_SDA1);\
                                  else	     GPIO_SetBits(I2C_GPIO1,I2C_PIN_SDA1)
#define Num1_SCL(x)		             if(x==0)  GPIO_ResetBits(I2C_GPIO1,I2C_PIN_SCL1);\
                                  else	 	 GPIO_SetBits(I2C_GPIO1,I2C_PIN_SCL1)
#define Num1_SDA_READ             GPIO_ReadInputDataBit(I2C_GPIO1,I2C_PIN_SDA1)  
   

void I2C_Config(void);
void Delay(vu32 i);
void Delay5us(void);
void init_code(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(uint8_t ack);
void I2C_RecvACK(void);
void I2C_SendByte(uint8_t dat);
uint8_t I2C_RecvByte(void);
void I2C_WriteByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
uint8_t I2C_ReadByte(uint8_t SlaveAddress,uint8_t REG_Address);
                                  
#endif
