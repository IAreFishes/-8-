/*******************************************************************************
* @file    id_protect.c 
* @author  hc
* @version V1.0
* @date    2015/10/7
* @brief   none									
*******************************************************************************/
#include "includes.h"

/**
  * @brief 芯片ID号保护
  * @param  void
  * @retval 如果ID验证没有通过将无限循环
  */
void Id_Protect_check(void)
{
	uint8_t i,ID_num[12];
	
/**************************ID保护****************************/
	for(i=0;i<12;i++)
	{
		const uint8_t ID_Value[12]={255,219,5,66,70,54,54,66,113,25,67,255};  //芯片ID
		ID_num[i]=*(u8 *)(0x1FFFF7E9+i);

			if(ID_num[i]!=ID_Value[i])
			{
				while(1){}
			}
			else if(ID_num[11]==ID_Value[11])
			{
				break;
			}
	}

/**************************ID保护****************************/
}
