/*******************************************************************************
* @file    TCS230.c 
* @author  hc
* @version V2.0
* @date    2016/3/23
* @brief   �������㷨							
*******************************************************************************/
#include "includes.h"

uint16_t TCS230_Period[8][VDATA_NUM+1]={0};  //���RGBԭʼ��������

volatile uint16_t R_Val[8]={0};  //Rֵ
volatile uint16_t G_Val[8]={0};  //Gֵ
volatile uint16_t B_Val[8]={0};  //Bֵ

uint32_t R_Pro[8][VDATA_NUM]={0};  //R��ƽ�����ֵ
uint32_t G_Pro[8][VDATA_NUM]={0};  //G��ƽ�����ֵ
uint32_t B_Pro[8][VDATA_NUM]={0};  //B��ƽ�����ֵ

volatile char adjust_direction = 0; //����1Ϊ�󣬷���2Ϊ�� �ư���Ҫ�����ķ���
volatile u16 adjust_distance =0;   //��������
volatile char white_line_num = 0;  //������
volatile u8 colour_flag = 0;  //��ɫ��־
volatile u8 colour_flag_num = 0; //����ǵ���ɫ��־

volatile char WhiteBalance_status = 0; //��ƽ������־λ

Discern_Type R_Type[ID_NUM_COLOR],G_Type[ID_NUM_COLOR],B_Type[ID_NUM_COLOR];
Discern_Type R_Type_Now,G_Type_Now,B_Type_Now;
uint16_t RefValue_min = 0;   //���ڼ��㵱ǰƫ�����ֵ��Сֵ
float RG_Ratio_Ref[ID_NUM_COLOR],GB_Ratio_Ref[ID_NUM_COLOR],RB_Ratio_Ref[ID_NUM_COLOR];












	float 	 test_i[8] = {0};









/**
  * @brief  TCS230��ʼ������
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
    
	GPIO_Config(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP);  	//��������LED
    GPIO_Config(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP);     //LED0
    
    GPIO_Config(GPIOA,GPIO_Pin_12,GPIO_Mode_IPD);		//��ƽ�ⰴ��
    
	GPIO_Config(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP);  // USART1_LED_CK
	GPIO_Config(GPIOA,GPIO_Pin_9,GPIO_Mode_Out_PP);  // USART1_LED_DR
    
    Output_Frequency(20);                        //��Դ���Ƶ��Ϊ 20%
    Filter_Set('N'); 							 //�ر��˲���
    
    TCS230_LED(1);              //����LED����ƿ���
    TCS230_Error_LED(1);        //�����źŵƹر�
    LightBoard_RedLed(0xff);    //8���źŵƿ���
    
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
  * @brief  ���Ƶ��
  * @param  uint8_t fre ���pinlv
  * @retval 0 Ϊ�ضϵ�Դ  2���2%  20���20%  100���100%
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
  * @brief  �˲�����������
  * @param  char RGB  ѡ��R G B �˲���
  * @retval ע���������Ϊ�ַ�����
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
  * @brief  �ư�8��LED 74HC164��λ���ƺ���
  * @param  char data  ���������
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
  * @brief  �������ݴ���
  * @param  char data  ���������
  * @retval �Ƚ�������ǰ��2�����ݵĴ�С���Դ����жϸ�ʹ�����ּ��㷽ʽ
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
  * @brief  ��ֵ�˲�����
  * @param  uint16_t *std ���� ��uint8_t n �����С
  * @retval ��ȡһ�����ݵ���ֵ
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
  * @brief  ��ֵ�˲�����
  * @param  uint32_t *std ���� ��uint8_t n �����С
  * @retval ��ȡһ�����ݵ���ֵ
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
  * @brief  ��ֵ�˲�����
  * @param  uint32_t *std ���� ��uint8_t n �����С
  * @retval ��ȡһ�����ݵ���ֵ
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
  * @brief  ��ƽ��
  * @param  Value ����ɫ��ƽ��Ĳο���ֵ
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
        Filter_Set('R');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            R_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            R_Pro[i][j] = RGB_Pro_Check(Value,R_Val[i]);   //���RGB��ֵ ���ذ�ƽ�����ϵ��
        }
        Filter_Set('G');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            G_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            G_Pro[i][j] = RGB_Pro_Check(Value,G_Val[i]);   //���RGB��ֵ ���ذ�ƽ�����ϵ��
        }
        
        Filter_Set('B');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            B_Val[i] = Data_Manage(TCS230_Period[i],VDATA_NUM);
            B_Pro[i][j] = RGB_Pro_Check(Value,B_Val[i]);   //���RGB��ֵ ���ذ�ƽ�����ϵ��
        }
    }
    for(i=0;i<VDATA_NUM;i++)
	{
		    Mid_Value_uint32_t(R_Pro[i],VDATA_NUM);
        Mid_Value_uint32_t(G_Pro[i],VDATA_NUM);
        Mid_Value_uint32_t(B_Pro[i],VDATA_NUM);
	}
    
    
    Flash_Erase_Page(60,1);		 //����flash�ĵ�60ҳ   һ��Ҫ�������ܴ������ݣ� ��Ȼ�޷����� 
	for(i=0;i<8;i++)			 //���������Ӵ���FLASH ��
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
  * @brief  TCS230�ö�ʱ�����벶��ģʽ
  * @param  
  * @retval None
  */
void TCS230_CAP_Task(void)
{    
    char i = 0;  
    for(i=0;i<8;i++)	 //�ȶ�ȡflash����ı�������һ��
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
        if(WhiteBalance_status==1)	  //�ж��Ƿ�����˰�ƽ�����
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
        Filter_Set('R');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
            
        for(i=0;i<=7;i++)
        {       
            R_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);    //�鿴ԭʼ����
            R_Val[i]= R_Pro[i][0]/R_Val[i];
        }
        
        Filter_Set('G');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            G_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);    //�鿴ԭʼ����
            G_Val[i]= G_Pro[i][0]/G_Val[i];
        }
        
        Filter_Set('B');  //������ɫ�˲���
        TIM2->DIER |= TIM_IT_CC1;
        TIM2->DIER |= TIM_IT_CC2;
        TIM2->DIER |= TIM_IT_CC3;
        TIM3->DIER |= TIM_IT_CC1;
        TIM3->DIER |= TIM_IT_CC2;
        TIM3->DIER |= TIM_IT_CC3;
        TIM4->DIER |= TIM_IT_CC1;
        TIM4->DIER |= TIM_IT_CC2;
        while(TIM_CCR_Status!=0xff){}  //�ȴ����б�־λ
        TIM_CCR_Status = 0;   
        for(i=0;i<=7;i++)
        {
            B_Val[i]= Data_Manage(TCS230_Period[i],VDATA_NUM);  //�鿴ԭʼ����   
            B_Val[i]= B_Pro[i][0]/B_Val[i];
        }   
        OSSemPost(TCS230_Semp);  // TCS230�������
        OSTimeDlyHMSM(0,0,0,2);
    }
}

/**
  * @brief  ��������У��
  * @param  Void
  * @retval None
  */
uint32_t RGB_Pro_Check(uint16_t Value,uint16_t RGB_Val)
{
	uint32_t RGB_Pro = 0;
	if(RGB_Val==0)  // ���RGB��ֵΪ0 ��Ϊ�쳣 ������ϵ����ֵΪ0
	{
		RGB_Pro=0;
	}
	else
	{
		RGB_Pro = RGB_Val*Value;     //����������Ӳ����
	}
	return RGB_Pro;
}

/**
  * @brief  �ư���ȡ����ƫ���
  * @param  Discern_Type type  �Զ���ĳ���������������
  * @retval None								
  */
uint16_t White_Line_Deviation(Discern_Type type)
{
    uint8_t i,j,l,max_site = 0,min_site = 0;
    uint16_t max = 0,min = 0;
	uint16_t average = 0;
    u16 distance = 0;   //��������
	float StdDev = 0;	   	//��׼��
    float K = 0;



	test_i[0] = 	 type.colour[0];
	test_i[1] = 	 type.colour[1];
	test_i[2] = 	 type.colour[2];
	test_i[3] = 	 type.colour[3];
	test_i[4] = 	 type.colour[4];
	test_i[5] = 	 type.colour[5];
	test_i[6] = 	 type.colour[6];
	test_i[7] = 	 type.colour[7];

    for(i=0,max = type.colour[0],min = type.colour[0];i<7;i++)  //�õ����ֵλ�� /*Ҫ�õ���Сֵ��λ��,����⵽���ߵ�λ��*/
	{
        if(type.colour[i+1] >  max)   										/*���Ѳ���ߣ�����Ѱ�����ֵ�ķ����������_ע��*/
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

	//��ֵ�������ж��������
	for(j = 0;j < 8;j++)
	{
		average += type.colour[j];			
	}	
	average = average / 8;

	//��ȡ��׼�������ж��Ƿ���
	for(l = 0;l < 8;l++)
	{
		StdDev += (type.colour[l] - average) * (type.colour[l] - average);
	}
   	StdDev = sqrt(StdDev / 8);
	

	type.min = type.colour[min_site];
	type.max = type.colour[max_site];

	//����
    LightBoard_RedLed(1<<min_site);

   	//ˢ��Kֵ
    K = (type.max - type.min) / DWINDOW;       

	//����
	if(StdDev < 50)		
	{
		return distance = 90;
	}
	else
	{
		//δ���ߣ����жϾ�ƫ����
		if(min_site <= 3)
		{
			adjust_direction = 1;	
		}
		else
		{
			adjust_direction = 2;
		}
	}
	/*ע�ͣ�һ���ú�ɫ���Сһ��Ĵ�����(��ֵ��һ�������)ȥ�������*/
	//���⴦��
	if(min_site == 0)//���
	{
		//������0,1������֮�䣬ƫ0��������������1������������롣
	 	if(type.colour[min_site + 1] < average )
		{
			//distance = /*�������� + */DWINDOW * 3.0 - ((type.max - type.colour[min_site + 1])/K) + DWINDOW / 2;		
			distance = DWINDOW * 3.0 + (DWINDOW / 2 - ((type.max - type.colour[min_site + 1])/K));
		}
		//�����������
		else
		{
			//distance = /*��������+ */DWINDOW * 4.0 - ((type.max - type.colour[min_site])/K) + DWINDOW / 2;
			distance = DWINDOW * 3.0 + (DWINDOW - ((type.max - type.colour[min_site])/K));
		}
		
		return distance;	
	}
	else if(min_site == 7)//�Ҷ�
	{
		 //������6,7������֮�䣬ƫ7��������
		 if(type.colour[min_site - 1] < average)
		 {
			//distance = /*�������� + */DWINDOW * 3.0 - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = 	DWINDOW * 3.0 +  (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		 }
		 //
		 else
		 {
			//distance = /*�������� + */DWINDOW * 4.0 - ((type.max - type.colour[min_site]) / K) + DWINDOW / 2;
			distance = 	DWINDOW * 3.0 +   (DWINDOW - ((type.max - type.colour[min_site]) / K));
		 }

	     return distance;
	}
#if 0
	//����Ѳ��
    if(min_site <= 3)	//����
	{
		//�ڱ궨�����
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*�������� + */DWINDOW * (3- min_site) + ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;	
			distance =  DWINDOW * (3 - min_site + 1)	- (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K)); 	
		}
		//�ұ�
		else
		{
			//distance = /*�������� + */DWINDOW * (3 - min_site) - ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
			distance =  DWINDOW * (3 - min_site)	+ (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K));
		}
		
		return distance;		
	}
	else  //�Ұ��
	{
		//�ڱ궨�����
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*�������� + */DWINDOW * (min_site - 4) - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = DWINDOW * (min_site - 4) + (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));		
		}
		//�ұ�
		else
		{
			//distance = /*�������� + */DWINDOW *(min_site - 4) + ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
		 	distance = DWINDOW *(min_site - 4 + 1) - (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		}

    	return distance;		
	}

#endif

#if 1
	 //�����
    if(min_site <= 3)	//����
	{
		//�ڱ궨�����
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*�������� + */DWINDOW * (3- min_site) + ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;	
			distance =  DWINDOW * (3 - min_site)	- (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K)); 	
		}
		//�ұ�
		else
		{
			//distance = /*�������� + */DWINDOW * (3 - min_site) - ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
			distance =  DWINDOW * (3 - min_site)	+ (DWINDOW / 2 - ((type.max - type.colour[min_site + 1]) / K));
		}
		
		return distance;		
	}
	else  //�Ұ��
	{
		//�ڱ궨�����
		if(type.colour[min_site - 1] < type.colour[min_site + 1])
		{
			//distance = /*�������� + */DWINDOW * (min_site - 4) - ((type.max - type.colour[min_site - 1]) / K) + DWINDOW / 2;
			distance = DWINDOW * (min_site - 4) + (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));		
		}
		//�ұ�
		else
		{
			//distance = /*�������� + */DWINDOW *(min_site - 4) + ((type.max - type.colour[min_site + 1]) / K) + DWINDOW / 2;
		 	distance = DWINDOW *(min_site - 4) - (DWINDOW / 2 - ((type.max - type.colour[min_site - 1]) / K));
		}

    	return distance;		
	}



#endif
}

/**
  * @brief  ɨ��һ������������Сֵ  �޶����ֵ��Сֵ�ķ�Χ65~1000
  * @param  uint16_t str  ���� uint8_t n�����С
  * @retval None								
  */
Discern_Type Scanning_Max_Min(uint16_t *std, uint8_t n)
{
    uint8_t i = 0,j = 0;
	uint16_t  m = 0;
	uint16_t Std[8] = {0};
    Discern_Type mod;
    
	
	for(i=0;i<n;i++)  //�����ݸ��Ƶ�Std
	{
		 Std[i] = std[i];
	}
   
	for(i=0;i<n-1;i++) //����  ��С����
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

	mod.max = Std[7];   //���ֵ
    mod.min = (Std[2]+Std[3])/2; 
	return mod;
}
/**
  * @brief  ȡ����ֵ
  * @param  int32_t num ȡ����ֵ����
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
  * @brief  �жϳ�����ɫ
  * @param  Void
  * @retval None								
  */
uint16_t Judge_Colour(void)
{
   
    char i = 0;
    uint16_t date = 0;
    uint16_t Ref_R = 0,Ref_G = 0,Ref_B = 0;
    uint16_t Ref_RGB = 0;
    float RG_Ratio = 0,GB_Ratio = 0,RB_Ratio = 0;//��ɫ����
    Discern_Type *Ref_Type;
    R_Type_Now = Scanning_Max_Min((uint16_t *)R_Val,8);Ref_R=R_Type_Now.min;  /*����Ӧ�øĳ����ֵ����ΪѲ���Ǻ��ߡ�֮ǰ����Сֵ������ΪҪ�ҵ��������������
																				��ɫ������ɫ���������������Сֵ�Ļ����ǿ϶��ǻ��ҵ����ߵ�_ע��*/
																				//��Ҫ�Ļ�����������
    G_Type_Now = Scanning_Max_Min((uint16_t *)G_Val,8);Ref_G=G_Type_Now.min;
    B_Type_Now = Scanning_Max_Min((uint16_t *)B_Val,8);Ref_B=B_Type_Now.min;
    RG_Ratio = Ref_R/Ref_G;
    GB_Ratio = Ref_G/Ref_B;
    RB_Ratio = Ref_R/Ref_B;
    for(i=1;i<ID_NUM_COLOR;i++)  //����RGB�����ж���ʲô��ɫ       /*��û�к�ɫ�жϣ�_ע��*/
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
    
    if(Ref_R>Ref_G)     //�ڷ��������Bֵ������ ����ֻѡRG����С��ֵ��Ϊ�ο�
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
  * @brief  ����ɨ��
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
		Flash_Erase_Page(70,1);//����flash�ĵ�70ҳ  	
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
  * @brief  ����ɨ��
  * @param  Void
  * @retval None								
  */
void Key_Scan(void)
{
    char i = 0;
    OSTimeDlyHMSM(0,0,0,20);
    for(i=0;;i++)  //�ж��Ƿ���
    {			    
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
        {
            OSTimeDlyHMSM(0,0,0,50);
            if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
            {            
//                OSTaskResume(USART_TASK_PRIO);
                 OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=0;//�ᰴ����Ϊ�궨ģʽ ��һ�²�������
                break;
            }
      
        }
        if(i==20)  //1s���»ָ���ƽ������
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
	tx_message.Data[2] = id_dengban;     //�ư�ID
   tx_message.Data[3] = colour_flag;       //��ǰ���ڳ�����ɫ
	
	TransmitMailbox = CAN_Transmit(CAN1,&tx_message);

	while((CAN_TransmitStatus(CAN1,TransmitMailbox)	!= CANTXOK) && (i < 0xfff))  //�ȴ��������
	{
		i++ ;
	}
	if(i>=0xfff)        //��Ϣ���ͳ�ʱ
	{
		return 1;
	}
	return 0;
}

/**
  * @brief  CAN1��������
  * @param  void
  * @retval None
  */
uint8_t CAN1_ReceiveData(void)
{
	CanRxMsg RxMessage;
	RxMessage.StdId = 0x00;         //��ʼ������
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.DLC = 0;
	RxMessage.Data[0] = 0x00;
	RxMessage.Data[1] = 0x00;       //��ʼ������
	RxMessage.Data[2] = 0x00;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.StdId==LIGHT_BOARD_ID&&RxMessage.Data[2]==LIGHT_BOARD_ID)  //�ж��ǲ��Ǳ���ID
	{
		switch(RxMessage.Data[0])
        {
            case 0: OSTaskSuspend(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(0);break;  //ֹͣ�������� ����
            case 1: OSTaskResume(Bounded_Check_TASK_PRIO);OSTaskSuspend(Count_Ehite_Line_TASK_PRIO);TCS230_LED(1);break;   //Ѳ������  //����
            case 2: OSTaskSuspend(Bounded_Check_TASK_PRIO); OSTaskResume(Count_Ehite_Line_TASK_PRIO);break;  //����
            case 3: OSTaskResume(WhiteBalance_TASK_PRIO);  //�ָ���ƽ������
            case 4: OSTaskResume(Site_Scanning_TASK_PRIO);colour_flag_num=0;break;
            default : break;  
        }
        if(RxMessage.Data[0]>=5)//����5������Ϊ�궨ָ��  
        {
             colour_flag_num = RxMessage.Data[0]-4;OSTaskResume(Site_Scanning_TASK_PRIO);
        }
	}
	return 1;
}

/**
  * @brief  ����������
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
