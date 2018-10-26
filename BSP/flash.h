#ifndef __FLASH_H
#define __FLASH_H
#include "includes.h"
#define FLASH_CHIP_SIZE 128     // 单位K字节
#define FLASH_START_ADDRESS 0x08000000+1024*30  //从第30K的地方开始
ErrorStatus Flash_Erase_Page(unsigned char Page,unsigned char number);
ErrorStatus Flash_Write_32Data(uint32_t Address, uint32_t Data);
ErrorStatus Flash_Write_16Data(uint32_t Address, uint16_t Data);
uint32_t Read_Flash_Data(uint8_t  data_type, uint32_t address);
#endif
