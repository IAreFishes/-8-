/*******************************************************************************
* @file    I2C.c 
* @author  hc
* @version V1.0
* @date    2015/11/21
* @brief   none									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  IO模拟I2C
  * @param  void
  * @retval None
  */
void I2C_Config(void)
{
   //GPIO_Mode_Out_OD注意 ,SDA 一定得用开漏模式
   GPIO_Config(I2C_GPIO1, I2C_PIN_SCL1,GPIO_Mode_Out_PP);   // I2C_SCL
   GPIO_Config(I2C_GPIO1, I2C_PIN_SDA1,GPIO_Mode_Out_OD);   // I2C_SDA
}
/**
  * @brief  I2C通信延时5us函数
  * @param  void
  * @retval 不同的环境下延时参数不同
            单周期指令，72M环境下 为36
  */
void Delay(vu32 i)
{
	for(;i>0;i--);
}
void Delay5us(void)
{
	Delay(6);		 //这里延时太大会导致卡尔曼滤波的收敛变慢
}

/**
  * @brief  I2C总线不工作的情况下，SDA和SCL信号都为高电平
  * @param  void
  * @retval None          
  */
void init_code(void)  // SCL SDA全部拉高
{	
    Num1_SCL(1);
    Delay5us();
    Num1_SDA(1);
    Delay5us();
}
/**
  * @brief  发送起始条件 SCL为高电平时 SDA从高变低
  * @param  void
  * @retval None         
  */
void I2C_Start(void)
{
    Num1_SDA(1);    //拉高数据线
    Delay5us();     //延时
    Num1_SCL(1);    //拉高时钟线
    Delay5us();     //延时
    Num1_SDA(0);    //产生下降沿
    Delay5us();     //延时
}
/**
  * @brief  停止数据传输，SCL为高电平时 SDA从低变高
  * @param  void
  * @retval None          
  */
void I2C_Stop(void)
{
    Num1_SDA(0);    //拉低数据线
    Delay5us();     //延时
    Num1_SCL(1);    //拉高时钟线
    Delay5us();     //延时
    Num1_SDA(1);    //产生上升沿
    Delay5us();     //延时
}
/**
  * @brief  发送应答信号
  * @param  入口参数:ack (0:ACK 1:NAK)
  * @retval None          
  */
void I2C_SendACK(uint8_t ack)
{
    Num1_SDA(ack);  //写应答信号
	Delay5us();     //延时
    Num1_SCL(1);    //拉高时钟线
    Delay5us();     //延时
    Num1_SCL(0);    //拉低时钟线
    Delay5us();     //延时
}
/**
  * @brief  接收应答信号
  * @param  void
  * @retval None          
  */
void I2C_RecvACK(void)
{
    unsigned int i=0;
    Num1_SCL(1);        //拉高时钟线
	Delay5us();         //延时
    while(Num1_SDA_READ==1) 
    {
        i++;
        Delay5us();
        if(i==350)   //I2C应答超时
        {
            break;
        }
    }
    i=0;
    Num1_SCL(0);    //拉低时钟线
    Delay5us();     //延时
}
/**
  * @brief  I2C发送一个数据
  * @param  dat 要发送的数据
  * @retval None          
  */
void I2C_SendByte(uint8_t dat)
{
    uint8_t i,date;
    for (i=0; i<8; i++)         //8位计数器
    {
        date=(dat&0x80);
        Num1_SCL(0);                //拉低时钟线
        Delay5us();             //延时
        Num1_SDA(date);               //送数据口
        Delay5us();
        Num1_SCL(1);                //拉高时钟线
        Delay5us();             //延时
        dat <<= 1;              //移出数据的最高位
    }
    Num1_SCL(0);
    Delay5us();
    Num1_SDA(1);
    Delay5us();
    I2C_RecvACK();
}
/**
  * @brief  I2C接收一个数据
  * @param  void
  * @retval 返回接收的数据          
  */
uint8_t I2C_RecvByte(void)
{
    uint8_t i;
    uint8_t dat = 0;
    Num1_SCL(0);
    Delay5us();
    Num1_SDA(1);        //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++) //8位计数器
    {
        Num1_SCL(1);    //拉高时钟线
        Delay5us();     //延时
        dat=(dat<<1)|Num1_SDA_READ; //读数据               
        Num1_SCL(0);    //拉低时钟线
        Delay5us();     //延时
    }
    Delay5us();
    return dat;
}
/**
  * @brief  I2C向目标地址写一个8位数据
  * @param  SlaveAddress 从机地址，REG_Address 写寄存器地址，REG_data 数据
  * @retval 第1~7位为目标设备地址，第0位为通信方向0发送，1接收          
  */
void I2C_WriteByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止
}
/**
  * @brief  I2C向目标地址读取一个数据
  * @param  SlaveAddress 从机地址，REG_Address 读寄存器地址
  * @retval 第1~7位为目标设备地址，第0位为通信方向0发送，1接收          
  */
uint8_t I2C_ReadByte(uint8_t SlaveAddress,uint8_t REG_Address)
{
    uint8_t REG_data;
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
    I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
    REG_data=I2C_RecvByte();       //读出寄存器数据
    I2C_SendACK(1);                //接收应答信号
    I2C_Stop();  
    return REG_data;
}
