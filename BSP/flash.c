#include "includes.h"
/**
  * @brief  ����FLASHҳ��
  * @param  Page ��ʼ  number ��ҳ size FLASH��С ��λK
  * @retval None
  */
ErrorStatus Flash_Erase_Page(unsigned char Page,unsigned char number)
{
	int i,k;
	k=Page;
	for(i=0;i<number;i++)
	{
		FLASH_Unlock();	   //����FLASH��д������
		FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP|FLASH_FLAG_BSY);  //�����־λ FLASH��д�����־��ҳ��д���������־������������־
		if(k>FLASH_CHIP_SIZE)  //���ݳ���оƬ��ַ��Χ
		{
			return ERROR;
		}
		FLASH_ErasePage(0x08000000+(k*1024));  //����һ��FLASHҳ��  ÿ��ҳ���С1K
		k++;
		FLASH_Lock();      //����FLASH��д������
	}
	return SUCCESS ;
}	

/**
  * @brief  ��ָ����ַд������
  * @param  Address ��ַ  Data ���� ��λ32λ
  * @retval None
  */
ErrorStatus Flash_Write_32Data(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();	   //����FLASH��д������
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);  //�����־λ FLASH��д�����־��ҳ��д���������־������������־
	if((Address-0x08000000)/1024>FLASH_CHIP_SIZE) //���ݳ���оƬ��ַ��Χ
	{
		return ERROR;  
	}
	FLASH_ProgramWord(Address,Data);	 		//��ָ��λ�ñ�дһ����
	while(FLASH_GetStatus()==FLASH_BUSY);		//�ȴ����ݸ���
	FLASH_ClearFlag(FLASH_FLAG_EOP);
	FLASH_Lock();      //����FLASH��д������
	return SUCCESS ;
}

/**
  * @brief  ��ָ����ַд������
  * @param  Address ��ַ  Data ���� ��λ16λ
  * @retval None
  */
ErrorStatus Flash_Write_16Data(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();	   //����FLASH��д������
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);  //�����־λ FLASH��д�����־��ҳ��д���������־������������־
	if((Address-0x08000000)/1024>FLASH_CHIP_SIZE) //���ݳ���оƬ��ַ��Χ
	{
		return ERROR;  
	}
	FLASH_ProgramHalfWord(Address,Data);	 	//��ָ��λ�ñ�дһ������
	while(FLASH_GetStatus()==FLASH_BUSY);		//�ȴ����ݸ���
	FLASH_ClearFlag(FLASH_FLAG_EOP);
	FLASH_Lock();      //����FLASH��д������
	return SUCCESS ;
}


/**
  * @brief  ��ȡFlash�е�����
  * @param  data_type ��ȡ���������� 8λ 16λ 32λ Add ��ַ 
  * @retval 
  */
uint32_t Read_Flash_Data(uint8_t  data_type, uint32_t address)
{
	uint32_t data = 0;
	if((address-0x08000000)/1024>FLASH_CHIP_SIZE) //���ݳ���оƬ��ַ��Χ
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
