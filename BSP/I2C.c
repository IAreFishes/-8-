/*******************************************************************************
* @file    I2C.c 
* @author  hc
* @version V1.0
* @date    2015/11/21
* @brief   none									
*******************************************************************************/
#include "includes.h"
/**
  * @brief  IOģ��I2C
  * @param  void
  * @retval None
  */
void I2C_Config(void)
{
   //GPIO_Mode_Out_ODע�� ,SDA һ�����ÿ�©ģʽ
   GPIO_Config(I2C_GPIO1, I2C_PIN_SCL1,GPIO_Mode_Out_PP);   // I2C_SCL
   GPIO_Config(I2C_GPIO1, I2C_PIN_SDA1,GPIO_Mode_Out_OD);   // I2C_SDA
}
/**
  * @brief  I2Cͨ����ʱ5us����
  * @param  void
  * @retval ��ͬ�Ļ�������ʱ������ͬ
            ������ָ�72M������ Ϊ36
  */
void Delay(vu32 i)
{
	for(;i>0;i--);
}
void Delay5us(void)
{
	Delay(6);		 //������ʱ̫��ᵼ�¿������˲�����������
}

/**
  * @brief  I2C���߲�����������£�SDA��SCL�źŶ�Ϊ�ߵ�ƽ
  * @param  void
  * @retval None          
  */
void init_code(void)  // SCL SDAȫ������
{	
    Num1_SCL(1);
    Delay5us();
    Num1_SDA(1);
    Delay5us();
}
/**
  * @brief  ������ʼ���� SCLΪ�ߵ�ƽʱ SDA�Ӹ߱��
  * @param  void
  * @retval None         
  */
void I2C_Start(void)
{
    Num1_SDA(1);    //����������
    Delay5us();     //��ʱ
    Num1_SCL(1);    //����ʱ����
    Delay5us();     //��ʱ
    Num1_SDA(0);    //�����½���
    Delay5us();     //��ʱ
}
/**
  * @brief  ֹͣ���ݴ��䣬SCLΪ�ߵ�ƽʱ SDA�ӵͱ��
  * @param  void
  * @retval None          
  */
void I2C_Stop(void)
{
    Num1_SDA(0);    //����������
    Delay5us();     //��ʱ
    Num1_SCL(1);    //����ʱ����
    Delay5us();     //��ʱ
    Num1_SDA(1);    //����������
    Delay5us();     //��ʱ
}
/**
  * @brief  ����Ӧ���ź�
  * @param  ��ڲ���:ack (0:ACK 1:NAK)
  * @retval None          
  */
void I2C_SendACK(uint8_t ack)
{
    Num1_SDA(ack);  //дӦ���ź�
	Delay5us();     //��ʱ
    Num1_SCL(1);    //����ʱ����
    Delay5us();     //��ʱ
    Num1_SCL(0);    //����ʱ����
    Delay5us();     //��ʱ
}
/**
  * @brief  ����Ӧ���ź�
  * @param  void
  * @retval None          
  */
void I2C_RecvACK(void)
{
    unsigned int i=0;
    Num1_SCL(1);        //����ʱ����
	Delay5us();         //��ʱ
    while(Num1_SDA_READ==1) 
    {
        i++;
        Delay5us();
        if(i==350)   //I2CӦ��ʱ
        {
            break;
        }
    }
    i=0;
    Num1_SCL(0);    //����ʱ����
    Delay5us();     //��ʱ
}
/**
  * @brief  I2C����һ������
  * @param  dat Ҫ���͵�����
  * @retval None          
  */
void I2C_SendByte(uint8_t dat)
{
    uint8_t i,date;
    for (i=0; i<8; i++)         //8λ������
    {
        date=(dat&0x80);
        Num1_SCL(0);                //����ʱ����
        Delay5us();             //��ʱ
        Num1_SDA(date);               //�����ݿ�
        Delay5us();
        Num1_SCL(1);                //����ʱ����
        Delay5us();             //��ʱ
        dat <<= 1;              //�Ƴ����ݵ����λ
    }
    Num1_SCL(0);
    Delay5us();
    Num1_SDA(1);
    Delay5us();
    I2C_RecvACK();
}
/**
  * @brief  I2C����һ������
  * @param  void
  * @retval ���ؽ��յ�����          
  */
uint8_t I2C_RecvByte(void)
{
    uint8_t i;
    uint8_t dat = 0;
    Num1_SCL(0);
    Delay5us();
    Num1_SDA(1);        //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++) //8λ������
    {
        Num1_SCL(1);    //����ʱ����
        Delay5us();     //��ʱ
        dat=(dat<<1)|Num1_SDA_READ; //������               
        Num1_SCL(0);    //����ʱ����
        Delay5us();     //��ʱ
    }
    Delay5us();
    return dat;
}
/**
  * @brief  I2C��Ŀ���ַдһ��8λ����
  * @param  SlaveAddress �ӻ���ַ��REG_Address д�Ĵ�����ַ��REG_data ����
  * @retval ��1~7λΪĿ���豸��ַ����0λΪͨ�ŷ���0���ͣ�1����          
  */
void I2C_WriteByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    I2C_Stop();                   //����ֹͣ
}
/**
  * @brief  I2C��Ŀ���ַ��ȡһ������
  * @param  SlaveAddress �ӻ���ַ��REG_Address ���Ĵ�����ַ
  * @retval ��1~7λΪĿ���豸��ַ����0λΪͨ�ŷ���0���ͣ�1����          
  */
uint8_t I2C_ReadByte(uint8_t SlaveAddress,uint8_t REG_Address)
{
    uint8_t REG_data;
    I2C_Start();                   //��ʼ�ź�
    I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
    I2C_Start();                   //��ʼ�ź�
    I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
    REG_data=I2C_RecvByte();       //�����Ĵ�������
    I2C_SendACK(1);                //����Ӧ���ź�
    I2C_Stop();  
    return REG_data;
}
