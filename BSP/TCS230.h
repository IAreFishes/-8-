#ifndef __TCS230_H
#define __TCS230_H

#include "includes.h"
#define LIGHT_BOARD_ID   LIGHT_BOARD_ID2    //�ư屾��ID                                        
#define LIGHT_BOARD_ID1  5	
#define LIGHT_BOARD_ID2  6	
#define LIGHT_BOARD_ID3  7		

#define VDATA_NUM   (6)       //�ɼ�6��ԭʼ����������
#define VDATA_NUM_S (5)       //�궨ʹ��5������������
#define VDATA_FLAT  (WHITE_WID-DWINDOW)  //ƽ��5mm��ȡ�����ÿ����ɫ�Ŀ����һ�µ�
#define WHITE_WID   (30)      //���߿� 30mm
#define DWINDOW     (20.0)    //��ⴰ20mm��������30mm�߶���
#define DEAD_ZONE   (50)      //ä����ֵ
#define C_THRESHOLD (100)     //�ж���ɫ��RGB��ֵ�ķ�ֵ
#define RATIO_THRESHOLD (0.05)//�ж���ɫ��RGB������ֵ  
#define ID_NUM_COLOR  (7)     //Ҫʶ�����ɫ��Ŀ

#define CHANGE_THR  (50)       // �ж���ɫ�ı䷧ֵ
#define CHANGE_NUM   (2)       //�ı���ٴ���Ч

typedef struct 
{
    uint8_t flat;
    uint16_t max;
    uint16_t min;
    uint16_t *colour;
}Discern_Type;

extern uint16_t TCS230_Period[8][VDATA_NUM+1]; // ���RGBԭʼ����
extern volatile uint16_t R_Val[8];  //Rֵ
extern volatile uint16_t G_Val[8];  //Gֵ
extern volatile uint16_t B_Val[8];  //Bֵ

extern uint32_t R_Pro[8][VDATA_NUM];  //R��ƽ�����ֵ
extern uint32_t G_Pro[8][VDATA_NUM];  //G��ƽ�����ֵ
extern uint32_t B_Pro[8][VDATA_NUM];  //B��ƽ�����ֵ

extern volatile char adjust_direction; //����1Ϊ�󣬷���2Ϊ�� �ư���Ҫ�����ķ���
extern volatile u16 adjust_distance;   //��������
extern volatile char white_line_num;  //������
extern volatile char WhiteBalance_status; //��ƽ�����־λ
extern volatile u8 colour_flag;
extern volatile u8 colour_flag_num;
extern volatile char num[8];
extern volatile uint8_t TIM_CCR_Status;
extern uint16_t RefValue_min;
extern float RGB_Ratio[ID_NUM_COLOR];
__inline void TCS230_S0(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_10);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_10);}
}
__inline void TCS230_S1(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_11);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_11);}
}
__inline void TCS230_S2(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_4);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_4);}
}
__inline void TCS230_S3(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_5);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_5);}
}
__inline void TCS230_LED(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_3);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_3);}
}
__inline void TCS230_Error_LED(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOB,GPIO_Pin_1);} 
	else{GPIO_ResetBits(GPIOB,GPIO_Pin_1);}
}
__inline void TCS230_CK_LED(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_8);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_8);}
}
__inline void TCS230_DR_LED(uint8_t sta)
{
    if(sta){GPIO_SetBits(GPIOA,GPIO_Pin_9);} 
	else{GPIO_ResetBits(GPIOA,GPIO_Pin_9);}
}

void TCS230_Config(void);
void Output_Frequency(uint8_t fre);
void Filter_Set(char RGB);
void LightBoard_RedLed(char data);
uint16_t Data_Manage(uint16_t *str,char size);
uint16_t Mid_Value(uint16_t *std,uint8_t n);
uint16_t Mid_Value_uint16_t(uint16_t *std,uint8_t n);
uint32_t Mid_Value_uint32_t(uint32_t *std,uint8_t n);
void White_Balance(uint32_t Value);
void TCS230_CAP_Task(void);
uint32_t RGB_Pro_Check(uint16_t Value,uint16_t RGB_Val);
Discern_Type Scanning_Max_Min(uint16_t *std, uint8_t n);
uint16_t White_Line_Deviation(Discern_Type type);
int32_t ABS(int32_t num);
uint16_t Judge_Colour(void);
uint8_t Site_Scanning(uint8_t num);
void Key_Scan(void);
uint8_t CAN1_SendData(unsigned char id_dengban,uint16_t result);
uint8_t CAN1_ReceiveData(void);
void Count_white_Line(void);

#endif
