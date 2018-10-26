#include "includes.h"
/**
  * @brief  擦除FLASH页面
  * @param  Page 起始  number 几页 size FLASH大小 单位K
  * @retval None
  */
ErrorStatus Flash_Erase_Page(unsigned char Page,unsigned char number)
{
	int i,k;
	k=Page;
	for(i=0;i<number;i++)
	{
		FLASH_Unlock();	   //解锁FLASH编写控制器
		FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP|FLASH_FLAG_BSY);  //清除标志位 FLASH编写错误标志，页面写保护错误标志，操作结束标志
		if(k>FLASH_CHIP_SIZE)  //数据超过芯片地址范围
		{
			return ERROR;
		}
		FLASH_ErasePage(0x08000000+(k*1024));  //擦除一个FLASH页面  每个页面大小1K
		k++;
		FLASH_Lock();      //锁定FLASH编写控制器
	}
	return SUCCESS ;
}	

/**
  * @brief  向指定地址写入数据
  * @param  Address 地址  Data 数据 单位32位
  * @retval None
  */
ErrorStatus Flash_Write_32Data(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();	   //解锁FLASH编写控制器
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);  //清除标志位 FLASH编写错误标志，页面写保护错误标志，操作结束标志
	if((Address-0x08000000)/1024>FLASH_CHIP_SIZE) //数据超过芯片地址范围
	{
		return ERROR;  
	}
	FLASH_ProgramWord(Address,Data);	 		//在指定位置编写一个字
	while(FLASH_GetStatus()==FLASH_BUSY);		//等待数据更新
	FLASH_ClearFlag(FLASH_FLAG_EOP);
	FLASH_Lock();      //锁定FLASH编写控制器
	return SUCCESS ;
}

/**
  * @brief  向指定地址写入数据
  * @param  Address 地址  Data 数据 单位16位
  * @retval None
  */
ErrorStatus Flash_Write_16Data(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();	   //解锁FLASH编写控制器
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);  //清除标志位 FLASH编写错误标志，页面写保护错误标志，操作结束标志
	if((Address-0x08000000)/1024>FLASH_CHIP_SIZE) //数据超过芯片地址范围
	{
		return ERROR;  
	}
	FLASH_ProgramHalfWord(Address,Data);	 	//在指定位置编写一个半字
	while(FLASH_GetStatus()==FLASH_BUSY);		//等待数据更新
	FLASH_ClearFlag(FLASH_FLAG_EOP);
	FLASH_Lock();      //锁定FLASH编写控制器
	return SUCCESS ;
}


/**
  * @brief  读取Flash中的数据
  * @param  data_type 读取的数据类型 8位 16位 32位 Add 地址 
  * @retval 
  */
uint32_t Read_Flash_Data(uint8_t  data_type, uint32_t address)
{
	uint32_t data = 0;
	if((address-0x08000000)/1024>FLASH_CHIP_SIZE) //数据超过芯片地址范围
	{
		return 0;  
	}
	switch(data_type)
	{
		case 8 :data = *(uint8_t *)address;break;
		case 16:data = *(uint16_t *)address;break;
		case 32:data = *(uint32_t *)address;break;
	}
	return data;
}
