/*******************************************************************************
* @file    TCS230.c 
* @author  hc
* @version V2.0
* @date    2016/3/23
* @brief   采用新算法							
*******************************************************************************/
#include "includes.h"

uint16_t TCS230_Period[8][VDATA_NUM+1]={0};  //存放RGB原始周期数据

volatile uint16_t R_Val[8]={0};  //R值
volatile uint16_t G_Val[8]={0};  //G值
volatile uint16_t B_Val[8]={0};  //B值

uint32_t R_Pro[8][VDATA_NUM]={0};  //R白平衡比例值
uint32_t G_Pro[8][VDATA_NUM]={0};  //G白平衡比例值
uint32_t B_Pro[8][VDATA_NUM]={0};  //B白平衡比例值

volatile char adjust_direction = 0; //方向1为左，方向2为右 灯板需要调整的方向
volatile u16 adjust_distance =0;   //调整距离
volatile char white_line_num = 0;  //白线数
volatile u8 colour_flag = 0;  //颜色标志
volatile u8 colour_flag_num = 0; //待标记的颜色标志

volatile char WhiteBalance_status = 0; //白平衡标完成志位

Discern_Type R_Type[ID_NUM_COLOR],G_Type[ID_NUM_COLOR],B_Type[ID_NUM_COLOR];
Discern_Type R_Type_Now,G_Type_Now,B_Type_Now;
uint16_t RefValue_min = 0;   //用于计算当前偏差的数值最小值
float RG_Ratio_Ref[ID_NUM_COLOR],GB_Ratio_Ref[ID_NUM_COLOR],RB_Ratio_Ref[ID_NUM_COLOR];












	float 	 test_i[8] = {0};









/**
  * @brief  TCS230初始化函数
  * @param  void 
  * @retval None
  */
void TCS230_Config(void)
{
    char num = 0;
    GPIO_Config(GPIOA,GPIO_Pin_10,GPIO_Mode_Out_PP);  	//S0
	GPIO_Config(GPIOA,GPIO_Pin_11,GPIO_Mode_Out_PP);    //S1
    GPIO_Config(GPIOA,GPIO_Pin_4,GPIO_Mode_Out_PP);  	//S2
	GPIO_Config(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP);		//S3
    
	GPIO_Config(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP);  	//控制所有LED
    GPIO_Config(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP);     //LED0
    
    GPIO_Config(GPIOA,GPIO_Pin_12,GPIO_Mode_IPD);		//白平衡按键
    
	GPIO_Config(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP);  // USART1_LED_CK
	GPIO_Config(GPIOA,GPIO_Pin_9,GPIO_Mode_Out_PP);  // USART1_LED_DR
    
    Output_Frequency(20);                        //电源输出频率为 20%
    Filter_Set('N'); 							 //关闭滤波器
    
    TCS230_LED(1);              //所有LED补光灯开启
    TCS230_Error_LED(1);        //错误信号灯关闭
    LightBoard_RedLed(0xff);    //8个信号灯开启
    
    for(num=1;num<ID_NUM_COLOR;num++)
    {
		R_Type[num].max=Read_Flash_Data(32,0x8011800+num*6*4+0*4);
		OSTimeDlyHMSM(0,0,0,1);
		R_Type[num].min=Read_Flash_Data(32,0x8011800+num*6*4+1*4);
		OSTimeDlyHMSM(0,0,0,1);
		G_Type[num].max=Read_Flash_Data(32,0x8011800+num*6*4+2*4);
		OSTimeDlyHMSM(0,0,0,1);
		G_Type[num].min=Read_Flash_Data(32,0x8011800+num*6*4+3*4);
		OSTimeDlyHMSM(0,0,0,1);
		B_Type[num].max=Read_Flash_Data(32,0x8011800+num*6*4+4*4);
		OSTimeDlyHMSM(0,0,0,1);
		B_Type[num].min=Read_Flash_Data(32,0x8011800+num*6*4+5*4);
		OSTimeDlyHMSM(0,0,0,1);
        
        R_Type[num].colour = (uint16_t *)R_Val;
        G_Type[num].colour = (uint16_t *)G_Val;
        B_Type[num].colour = (uint16_t *)B_Val;
             
        R_Type[num].flat = VDATA_FLAT;
        G_Type[num].flat = VDATA_FLAT;
        B_Type[num].flat = VDATA_FLAT;
    }
    for(num=0;num<ID_NUM_COLOR;num++)
    {
        RG_Ratio_Ref[num] = R_Type[num].min/G_Type[num].min;
        GB_Ratio_Ref[num] = G_Type[num].min/B_Type[num].min;
        RB_Ratio_Ref[num] = R_Type[num].min/B_Type[num].min;
    }        
}



/**
  * @brief  输出频率
  * @param  uint8_t fre 输出pinlv
  * @retval 0 为关断电源  2输出2%  20输出20%  100输出100%
  */
void Output_Frequency(uint8_t fre)
{
	switch(fre)
	{
		case 0:TCS230_S0(0);TCS230_S1(0);break;   
		case 2:TCS230_S0(0);TCS230_S1(1);break;
		case 20:TCS230_S0(1);TCS230_S1(0);break;
		case 100:TCS230_S0(1);TCS230_S1(1);break;
		default :break;
	}
}
/**
  * @brief  滤波器类型设置
  * @param  char RGB  选择R G B 滤波器
  * @retval 注意输入参数为字符类型
  */
void Filter_Set(char RGB)
{
	switch(RGB)
	{
		case 'R':TCS230_S2(0);TCS230_S3(0);break;
		case 'G':TCS230_S2(1);TCS230_S3(1);break;
		case 'B':TCS230_S2(0);TCS230_S3(1);break;
		case 'N':TCS230_S2(1);TCS230_S3(0);break;
		default :break;
	}
}

/**
  * @brief  灯板8个LED 74HC164移位控制函数
  * @param  char data  输入的数据
  * @retval None
  */
void LightBoard_RedLed(char data)
{
	char i = 0;
	for(i=0;i<=7;i++)
	{
		if((data&(0x01<<i))==0)
		{
			TCS230_DR_LED(0);
			TCS230_CK_LED(0);
			TCS230_CK_LED(1);
		}
		else
		{
			TCS230_DR_LED(1);
			TCS230_CK_LED(0);
			TCS230_CK_LED(1);
		}
	}
}
/**
  * @brief  传送数据处理
  * @param  char data  输入的数据
  * @retval 比较数组中前后2个数据的大小，以此来判断该使用哪种计算方式
  */
uint16_t Data_Manage(uint16_t *str,char size)
{
	char i = 0;
	for(i=0;i<size-1;i++)
	{
		if(str[i+1]>str[i])
		{
			str[i]=str[i+1]-str[i];
		}
		else
		{
			str[i]=str[i+1]+(0xffff-str[i]);
		}
	}
	return Mid_Value(str,size);	
}
/**
  * @brief  中值滤波函数
  * @param  uint16_t *std 数组 ，uint8_t n 数组大小
  * @retval 求取一组数据的中值
  */
uint16_t Mid_Value(uint16_t *std,uint8_t n)
{
	uint8_t i,j;
	uint16_t m = 0;
    uint32_t sum = 0;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(std[i]>std[j])
			{
				m = std[i];
				std[i] = std[j];
				std[j] = m;
			}
		}
	}
    for(i=0,j=0;i<n;i++)
    {
        if(ABS(std[i]-std[n/2])<=30)
        {
            sum+=std[i];
            j++;
        }
    }
    sum/=j;
	return sum;	
}
/**
  * @brief  中值滤波函数
  * @param  uint32_t *std 数组 ，uint8_t n 数组大小
  * @retval 求取一组数据的中值
  */
uint32_t Mid_Value_uint32_t(uint32_t *std,uint8_t n)
{
    uint8_t i,j;
	uint32_t m = 0;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(std[i]>std[j])
			{
				m = std[i];
				std[i] = std[j];
				std[j] = m;
			}
		}
	}
    return std[n/2];
}

/**
  * @brief  中值滤波函数
  * @param  uint32_t *std 数组 ，uint8_t n 数组大小
  * @retval 求取一组数据的中值
  */
uint16_t Mid_Value_uint16_t(uint16_t *std,uint8_t n)
{
    uint8_t i,j;
	uint32_t m = 0;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(std[i]>std[j])
			{
				m = std[i];
				std[i] = std[j];
				std[j] = m;
			}
		}
	}
    return std[n/2];
}
/**
  * @brief  白平衡
  * @param  Value 将白色白平衡的参考数值
  * @retval None
  */

void White_Balance(uint32_t Value)
{
	char i = 0,j = 0;
    
    
	for(i=0;i<3;i++)
	{
		TCS230_LED(0);
		OSTimeDlyHMSM(0,0,0,200);
		TCS230_LED(1);
		OSTimeDlyHMSM(0,0,0,200);
	}
    for(j=0;j<VDATA_NUM;j++)
    {
        TIM_CCR_Status = 0; 
        Filter_Set('R');  //开启红色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            R_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            R_Pro[i][j] = RGB_Pro_Check(Value,R_Val[i]);   //检查RGB数值 返回白平衡比例系数
        }
        Filter_Set('G');  //开启绿色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            G_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            G_Pro[i][j] = RGB_Pro_Check(Value,G_Val[i]);   //检查RGB数值 返回白平衡比例系数
        }
        
        Filter_Set('B');  //开启蓝色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            B_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            B_Pro[i][j] = RGB_Pro_Check(Value,B_Val[i]);   //检查RGB数值 返回白平衡比例系数
        }
    }
    for(i=0;i<VDATA_NUM;i++)
	{
		    Mid_Value_uint32_t(R_Pro[i],VDATA_NUM);
        Mid_Value_uint32_t(G_Pro[i],VDATA_NUM);
        Mid_Value_uint32_t(B_Pro[i],VDATA_NUM);
	}
    
    
    Flash_Erase_Page(60,1);		 //擦除flash的第60页   一定要擦除才能存入数据， 不然无法存入 
	for(i=0;i<8;i++)			 //将比例因子存入FLASH 中
	{
		Flash_Write_32Data(0x800F000+i*4,R_Pro[i][VDATA_NUM/2]);
		OSTimeDlyHMSM(0,0,0,1);	
	}
	OSTimeDlyHMSM(0,0,0,10);
	for(i=0;i<8;i++)
	{
		Flash_Write_32Data(0x800F000+32+i*4,G_Pro[i][VDATA_NUM/2]);	
		OSTimeDlyHMSM(0,0,0,1);	
	}
    OSTimeDlyHMSM(0,0,0,10);
	for(i=0;i<8;i++)
	{
		Flash_Write_32Data(0x800F000+64+i*4,B_Pro[i][VDATA_NUM/2]);	
		OSTimeDlyHMSM(0,0,0,1);	
	}
    WhiteBalance_status = 1;
	OSTimeDlyHMSM(0,0,0,20);
}

/**
  * @brief  TCS230用定时器输入捕获模式
  * @param  
  * @retval None
  */
void TCS230_CAP_Task(void)
{    
    char i = 0;  
    for(i=0;i<8;i++)	 //先读取flash里面的比例因子一次
	{
		R_Pro[i][0]=Read_Flash_Data(32,0x800F000+4*i);
	}
	OSTimeDlyHMSM(0,0,0,5);
    for(i=0;i<8;i++)
	{
		G_Pro[i][0]=Read_Flash_Data(32,0x800F000+32+4*i);	
	}
	OSTimeDlyHMSM(0,0,0,5);
    for(i=0;i<8;i++)
	{
		B_Pro[i][0]=Read_Flash_Data(32,0x800F000+64+4*i);	
	}
	OSTimeDlyHMSM(0,0,0,5);
    
	for(;;)
	{	
        if(WhiteBalance_status==1)	  //判断是否进行了白平衡操作
		{
			for(i=0;i<8;i++)
			{
				R_Pro[i][0]=Read_Flash_Data(32,0x800F000+4*i);	               
			}
			OSTimeDlyHMSM(0,0,0,5);
			for(i=0;i<8;i++)
			{
				G_Pro[i][0]=Read_Flash_Data(32,0x800F000+32+4*i);
			}
			OSTimeDlyHMSM(0,0,0,5);	
            for(i=0;i<8;i++)
			{
				B_Pro[i][0]=Read_Flash_Data(32,0x800F000+64+4*i);
			}
			OSTimeDlyHMSM(0,0,0,5);
			WhiteBalance_status=0;	
		}
        TIM_CCR_Status = 0;
        Filter_Set('R');  //开启红色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
            
        for(i=0;i<=7;i++)
        {       
            R_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);    //查看原始数据
            R_Val[i]= R_Pro[i][0]/R_Val[i];
        }
        
        Filter_Set('G');  //开启绿色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            G_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);    //查看原始数据
            G_Val[i]= G_Pro[i][0]/G_Val[i];
        }
        
        Filter_Set('B');  //开启蓝色滤波器
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //等待所有标志位
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            B_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);  //查看原始数据   
            B_Val[i]= B_Pro[i][0]/B_Val[i];
        }   
        OSSemPost(TCS230_Semp);  // TCS230任务完成
        OSTimeDlyHMSM(0,0,0,2);
    }
}

/**
  * @brief  比例因子校验
  * @param  Void
  * @retval None
  */
uint32_t RGB_Pro_Check(uint16_t Value,uint16_t RGB_Val)
{
	uint32_t RGB_Pro = 0;
	if(RGB_Val==0)  // 如果RGB数值为0 则为异常 将比例系数赋值为0
	{
		RGB_Pro=0;
	}
	else
	{
		RGB_Pro = RGB_Val*Value;     //计算比例因子并存放
	}
	return RGB_Pro;
}

/**
  * @brief  灯板求取白线偏差函数
  * @param  Discern_Type type  自定义的场地特征数据类型
  * @retval None								
  */
uint16_t White_Line_Deviation(Discern_Type type)
{
    uint8_t i,j,l,max_site = 0,min_site = 0;
    uint16_t max = 0,min = 0;
	uint16_t average = 0;
    u16 distance = 0;   //调整距离
	float StdDev = 0;	   	//标准差
    float K = 0;



	test_i[0] = 	 type.colour[0];
	test_i[1] = 	 type.colour[1];
	test_i[2] = 	 type.colour[2];
	test_i[3] = 	 type.colour[3];
	test_i[4] = 	 type.colour[4];
	test_i[5] = 	 type.colour[5];
	test_i[6] = 	 type.colour[6];
	test_i[7] = 	 type.colour[7];

    for(i=0,max = type.colour[0],min = type.colour[0];i<7;i++)  //得到最大值位置 /*要得到最小值的位置,即检测到黑线的位置*/
	{
        if(type.colour[i+1] >  max)   										/*如果巡黑线，这种寻找最大值的方法会出问题_注释*/
        {     
            max = type.colour[i+1];
            max_site = i+1;
        }
		if(type.colour[i+1] <  min)   										
        {     
            min = type.colour[i+1];
            min_site = i+1;
        }
	}

	//均值，用于判断特殊情况
	for(j = 0;j < 8;j++)
	{
		average += type.colour[j];			
	}	
	average = average / 8;

	//求取标准差用于判断是否丢线
	for(l = 0;l < 8;l++)
	{
		StdDev += (type.colour[l] - average) * (type.colour[l] - average);
	}
   	StdDev = sqrt(StdDev / 8);
	

	type.min = type.colour[min_site];
	type.max = type.colour[max_site];

	//亮灯
    LightBoard_RedLed(1<<min_site);

   	//刷新K值
    K = (type.max - type.min) / DWINDOW;       

	//丢线
	if(StdDev < 50)		
	{
		return distance = 90;
	}
	else
	{
		//未丢线，则判断纠偏方向
		if(min_site <= 3)
		{
			adjust_direction = 1;	
		}
		else
		{
			adjust_direction = 2;
		}
	}
	/*注释：一律用黑色面积小一点的传感器(数值大一点的数据)去估算距离*/
	//特殊处理
	if(min_site == 0)//左端
	{
		//黑线在0,1传感器之间，偏0传感器，所以用1传感器估算距离。
	 	if(type.colour[min_site + 1] < average )
		{
			//distance = /*基础长度 + */DWINDOW * 3.0 - ((type.max - type.colour[min_site + 1])/K) + DWINDOW / 2;		
			distance = DWINDOW * 3.0 + (DWINDOW / 2 - ((type.max - type.colour[min_site + 1])/K));
		}
		//黑线在最左端
		else
		{
			//distance = /*基础长度+ */DWINDOW * 4.0 - ((type.max - type.colour[min_site])/K) + DWINDOW / 2;
			distance = DWINDOW * 3.0 + (DWINDOW - ((type.max - type.colour[min_site])/K));
		}
		
		return distance;	
	}
	else if(min_site == 7)//右端
	{
		 //黑线在6,7传感器之间，偏7传感器。
		 if(type.colour[min_site - 1] < average)
		 {
			//distance = /*基础长度 + */DWINDOW * 3.0 - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = 	DWINDOW * 3.0 +  (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		 }
		 //
		 else
		 {
			//distance = /*基础长度 + */DWINDOW * 4.0 - ((type.max - type.colour[min_site]) / K) + DWINDOW / 2;
			distance = 	DWINDOW * 3.0 +   (DWINDOW - ((type.max - type.colour[min_site]) / K));
		 }

	     return distance;
	}
#if 0
	//正常巡线
    if(min_site <= 3)	//左半边
	{
		//在标定点左边
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*基础长度 + */DWINDOW * (3- min_site) + ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;	
			distance =  DWINDOW * (3 - min_site + 1)	- (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K)); 	
		}
		//右边
		else
		{
			//distance = /*基础长度 + */DWINDOW * (3 - min_site) - ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
			distance =  DWINDOW * (3 - min_site)	+ (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K));
		}
		
		return distance;		
	}
	else  //右半边
	{
		//在标定点左边
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*基础长度 + */DWINDOW * (min_site - 4) - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = DWINDOW * (min_site - 4) + (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));		
		}
		//右边
		else
		{
			//distance = /*基础长度 + */DWINDOW *(min_site - 4) + ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
		 	distance = DWINDOW *(min_site - 4 + 1) - (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		}

    	return distance;		
	}

#endif

#if 1
	 //间隔线
    if(min_site <= 3)	//左半边
	{
		//在标定点左边
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*基础长度 + */DWINDOW * (3- min_site) + ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;	
			distance =  DWINDOW * (3 - min_site)	- (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K)); 	
		}
		//右边
		else
		{
			//distance = /*基础长度 + */DWINDOW * (3 - min_site) - ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
			distance =  DWINDOW * (3 - min_site)	+ (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K));
		}
		
		return distance;		
	}
	else  //右半边
	{
		//在标定点左边
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*基础长度 + */DWINDOW * (min_site - 4) - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = DWINDOW * (min_site - 4) + (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));		
		}
		//右边
		else
		{
			//distance = /*基础长度 + */DWINDOW *(min_site - 4) + ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
		 	distance = DWINDOW *(min_site - 4) - (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		}

    	return distance;		
	}



#endif
}

/**
  * @brief  扫描一个数组的最大最小值  限定最大值最小值的范围65~1000
  * @param  uint16_t str  数组 uint8_t n数组大小
  * @retval None								
  */
Discern_Type Scanning_Max_Min(uint16_t *std, uint8_t n)
{
    uint8_t i = 0,j = 0;
	uint16_t  m = 0;
	uint16_t Std[8] = {0};
    Discern_Type mod;
    
	
	for(i=0;i<n;i++)  //将数据复制到Std
	{
		 Std[i] = std[i];
	}
   
	for(i=0;i<n-1;i++) //排序  从小到大
	{
		for(j=i+1;j<n;j++)
		{
			if(Std[i]>Std[j])
			{
				m = Std[i];
				Std[i] = Std[j];
				Std[j] = m;
			}
		}
	}

	mod.max = Std[7];   //最大值
    mod.min = (Std[2]+Std[3])/2; 
	return mod;
}
/**
  * @brief  取绝对值
  * @param  int32_t num 取绝对值的数
  * @retval None								
  */
int32_t ABS(int32_t num)
{
  if(num > 0)
  {
    return num;
  }
  else
  {
    return -num;
  }	  
}

/**
  * @brief  判断场地颜色
  * @param  Void
  * @retval None								
  */
uint16_t Judge_Colour(void)
{
   
    char i = 0;
    uint16_t date = 0;
    uint16_t Ref_R = 0,Ref_G = 0,Ref_B = 0;
    uint16_t Ref_RGB = 0;
    float RG_Ratio = 0,GB_Ratio = 0,RB_Ratio = 0;//颜色比率
    Discern_Type *Ref_Type;
    R_Type_Now = Scanning_Max_Min((uint16_t *)R_Val,8);Ref_R=R_Type_Now.min;  /*这里应该改成最大值，因为巡的是黑线。之前是最小值，是因为要找到除开白线以外的
																				深色场地颜色。现在如果还是最小值的话，那肯定是会找到黑线的_注释*/
																				//需要改回来，待测试
    G_Type_Now = Scanning_Max_Min((uint16_t *)G_Val,8);Ref_G=G_Type_Now.min;
    B_Type_Now = Scanning_Max_Min((uint16_t *)B_Val,8);Ref_B=B_Type_Now.min;
    RG_Ratio = Ref_R/Ref_G;
    GB_Ratio = Ref_G/Ref_B;
    RB_Ratio = Ref_R/Ref_B;
    for(i=1;i<ID_NUM_COLOR;i++)  //利用RGB比例判断是什么颜色       /*有没有黑色判断？_注释*/
    {
        if(
        fabs(RG_Ratio_Ref[i]-RG_Ratio)<=RATIO_THRESHOLD&&
        fabs(GB_Ratio_Ref[i]-GB_Ratio)<=RATIO_THRESHOLD&&
        fabs(RB_Ratio_Ref[i]-RB_Ratio)<=RATIO_THRESHOLD)
        {
            colour_flag = i;
            break;
        }
    }
    
    if(Ref_R>Ref_G)     //在反光材质上B值会跳变 所以只选RG中最小的值作为参考
    {
//        if(Ref_G>Ref_B)
//        {    
//            Ref_RGB = Ref_B;Ref_Type = B_Type;   
//        }
//        else
//        {
            Ref_RGB = Ref_G;Ref_Type = G_Type;
//        }
    }
//    else if(Ref_R>Ref_B)
//    {
//        Ref_RGB = Ref_B;Ref_Type = B_Type;   
//    }
    else
    { 
        Ref_RGB = Ref_R;Ref_Type = R_Type;
    }

    RefValue_min = Ref_RGB;
    date = White_Line_Deviation(Ref_Type[colour_flag]);
    return date;
}

/**
  * @brief  场地扫描
  * @param  Void
  * @retval None								
  */
uint8_t Site_Scanning(uint8_t num)
{
	   extern int Text;
    uint16_t R_Max[VDATA_NUM_S] = {0},R_Min[VDATA_NUM_S] = {0xffff,0xffff,0xffff,0xffff,0xffff};
    uint16_t G_Max[VDATA_NUM_S] = {0},G_Min[VDATA_NUM_S] = {0xffff,0xffff,0xffff,0xffff,0xffff};
    uint16_t B_Max[VDATA_NUM_S] = {0},B_Min[VDATA_NUM_S] = {0xffff,0xffff,0xffff,0xffff,0xffff};
    uint16_t i = 0,j = 0;
	OSTaskSuspend(Bounded_Check_TASK_PRIO);
    if(num==0)
    {
		Flash_Erase_Page(70,1);//擦除flash的第70页  	
        return 0;
    }
	 
 //   num=Text;
    for(j=0;j<VDATA_NUM_S;j++)
    {
        for(i=0;i<50;i++)
        { 
            R_Type[num] = Scanning_Max_Min((uint16_t *)R_Val,8);
            if(R_Max[j]<R_Type[num].max)
            {
                R_Max[j] = R_Type[num].max;
            }
            if(R_Min[j]>R_Type[num].min)
            {
                R_Min[j] = R_Type[num].min;
            }
            
            G_Type[num] = Scanning_Max_Min((uint16_t *)G_Val,8);
            if(G_Max[j]<G_Type[num].max)
            {
                G_Max[j] = G_Type[num].max;
            }
            if(G_Min[j]>G_Type[num].min)
            {
                G_Min[j] = G_Type[num].min;
            }
            
            B_Type[num] = Scanning_Max_Min((uint16_t *)B_Val,8);
            if(B_Max[j]<B_Type[num].max)
            {
                B_Max[j] = B_Type[num].max;
            }
            if(B_Min[j]>B_Type[num].min)
            {
                B_Min[j] = B_Type[num].min;
            }            
            OSTimeDlyHMSM(0,0,0,20);     
        }
    }    

    R_Type[num].max=Mid_Value_uint16_t(R_Max,VDATA_NUM_S);
    G_Type[num].max=Mid_Value_uint16_t(G_Max,VDATA_NUM_S);
    B_Type[num].max=Mid_Value_uint16_t(B_Max,VDATA_NUM_S);  
    R_Type[num].min=Mid_Value_uint16_t(R_Min,VDATA_NUM_S);
    G_Type[num].min=Mid_Value_uint16_t(G_Min,VDATA_NUM_S);
    B_Type[num].min=Mid_Value_uint16_t(B_Min,VDATA_NUM_S);
	
		
	Flash_Write_32Data(0x8011800+num*6*4+0*4,R_Type[num].max);
	OSTimeDlyHMSM(0,0,0,1);	
	Flash_Write_32Data(0x8011800+num*6*4+1*4,R_Type[num].min);
	OSTimeDlyHMSM(0,0,0,1);
	Flash_Write_32Data(0x8011800+num*6*4+2*4,G_Type[num].max);
	OSTimeDlyHMSM(0,0,0,1);
	Flash_Write_32Data(0x8011800+num*6*4+3*4,G_Type[num].min);
	OSTimeDlyHMSM(0,0,0,1);
	Flash_Write_32Data(0x8011800+num*6*4+4*4,B_Type[num].max);
	OSTimeDlyHMSM(0,0,0,1);
	Flash_Write_32Data(0x8011800+num*6*4+5*4,B_Type[num].min);
	OSTimeDlyHMSM(0,0,0,1);  
	return 0;
    
}

/**
  * @brief  场地扫描
  * @param  Void
  * @retval None								
  */
void Key_Scan(void)
{
    char i = 0;
    OSTimeDlyHMSM(0,0,0,20);
    for(i=0;;i++)  //判断是否按下
    {			    
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
        {
            OSTimeDlyHMSM(0,0,0,50);
            if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
            {            
//                OSTaskResume(USART_TASK_PRIO);
                 OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=0;//轻按以下为标定模式 第一下擦除数据
                break;
            }
      
        }
        if(i==20)  //1s按下恢复白平衡任务
        {
            OSTaskResume(WhiteBalance_TASK_PRIO);
            break;
        }
        OSTimeDlyHMSM(0,0,0,50);
    }
}


uint8_t CAN1_SendData(unsigned char id_dengban,uint16_t result)
{
	u8 TransmitMailbox=0;
	CanTxMsg tx_message;
	int i = 0;
	tx_message.StdId = id_dengban;
	tx_message.ExtId = 0;	
	tx_message.RTR = CAN_RTR_DATA;  	 //set to the data frame.
	tx_message.IDE = CAN_ID_STD;    	 //use standard ID
	tx_message.DLC =4;              	 //the length of the data,the longest data is 8; 
	tx_message.Data[0] = result;
	tx_message.Data[1] = adjust_direction;
	tx_message.Data[2] = id_dengban;     //灯板ID
   tx_message.Data[3] = colour_flag;       //当前所在场地颜色
	
	TransmitMailbox = CAN_Transmit(CAN1,&tx_message);

	while((CAN_TransmitStatus(CAN1,TransmitMailbox)	!= CANTXOK) && (i < 0xfff))  //等待发送完成
	{
		i++ ;
	}
	if(i>=0xfff)        //消息发送超时
	{
		return 1;
	}
	return 0;
}

/**
  * @brief  CAN1接收数据
  * @param  void
  * @retval None
  */
uint8_t CAN1_ReceiveData(void)
{
	CanRxMsg RxMessage;
	RxMessage.StdId = 0x00;         //初始化清零
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.DLC = 0;
	RxMessage.Data[0] = 0x00;
	RxMessage.Data[1] = 0x00;       //初始化清零
	RxMessage.Data[2] = 0x00;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.StdId==LIGHT_BOARD_ID&&RxMessage.Data[2]==LIGHT_BOARD_ID)  //判断是不是本机ID
	{
		switch(RxMessage.Data[0])
        {
            case 0: OSTaskSuspend(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(0);break;  //停止所有任务 待命
            case 1: OSTaskResume(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(1);break;   //巡线找线  //开灯
            case 2: OSTaskSuspend(Bounded_Check_TASK_PRIO); OSTaskResume(Count_Ehite_Line_TASK_PRIO);break;  //数线
            case 3: OSTaskResume(WhiteBalance_TASK_PRIO);  //恢复白平衡任务
            case 4: OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=0;break;
            default : break;  
        }
        if(RxMessage.Data[0]>=5)//大于5的命令为标定指令  
        {
             colour_flag_num = RxMessage.Data[0]-4;OSTaskResume(Site_Scanning_TASK_PRIO);
        }
	}
	return 1;
}

/**
  * @brief  数白线任务
  * @param  Void
  * @retval None								
  */
void Count_white_Line(void)
{
    R_Type_Now = Scanning_Max_Min((uint16_t *)R_Val,8);
    G_Type_Now = Scanning_Max_Min((uint16_t *)G_Val,8);
    B_Type_Now = Scanning_Max_Min((uint16_t *)B_Val,8);
   
    if((R_Type_Now.max-R_Type_Now.min)>200&&
       (G_Type_Now.max-G_Type_Now.min)>200&&
       (B_Type_Now.max-B_Type_Now.min)>200)
    {
        white_line_num++;
    }  
    OSTimeDlyHMSM(0,0,0,200);
}
