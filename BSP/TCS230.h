#ifndef __TCS230_H
#define __TCS230_H

#include "includes.h"
#define LIGHT_BOARD_ID   LIGHT_BOARD_ID2    //灯板本机ID                                        
#define LIGHT_BOARD_ID1  5	
#define LIGHT_BOARD_ID2  6	
#define LIGHT_BOARD_ID3  7		

#define VDATA_NUM   (6)       //采集6个原始数据做处理
#define VDATA_NUM_S (5)       //标定使用5个数据做处理
#define VDATA_FLAT  (WHITE_WID-DWINDOW)  //平地5mm宽度。假设每种颜色的宽度是一致的
#define WHITE_WID   (30)      //白线宽 30mm
#define DWINDOW     (20.0)    //检测窗20mm，传感器30mm高度下
#define DEAD_ZONE   (50)      //盲区阀值
#define C_THRESHOLD (100)     //判断颜色的RGB数值的阀值
#define RATIO_THRESHOLD (0.05)//判断颜色的RGB比例阀值  
#define ID_NUM_COLOR  (7)     //要识别的颜色数目

#define CHANGE_THR  (50)       // 判断颜色改变阀值
#define CHANGE_NUM   (2)       //改变多少次有效

typedef struct 
{
    uint8_t flat;
    uint16_t max;
    uint16_t min;
    uint16_t *colour;
}Discern_Type;

extern uint16_t TCS230_Period[8][VDATA_NUM+1]; // 存放RGB原始数据
extern volatile uint16_t R_Val[8];  //R值
extern volatile uint16_t G_Val[8];  //G值
extern volatile uint16_t B_Val[8];  //B值

extern uint32_t R_Pro[8][VDATA_NUM];  //R白平衡比例值
extern uint32_t G_Pro[8][VDATA_NUM];  //G白平衡比例值
extern uint32_t B_Pro[8][VDATA_NUM];  //B白平衡比例值

extern volatile char adjust_direction; //方向1为左，方向2为右 灯板需要调整的方向
extern volatile u16 adjust_distance;   //调整距离
extern volatile char white_line_num;  //白线数
extern volatile char WhiteBalance_status; //白平衡完成志位
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
