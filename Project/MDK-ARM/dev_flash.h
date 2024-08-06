#ifndef __DEV_FLASH_H
#define __DEV_FLASH_H 

#include "main.h"

#define FLASHSTORE 0x0800F000

union DATA_STORE
{
		uint32_t data;
		struct 
		{
			uint8_t Frequency;
			uint8_t Vibration;
			uint8_t reserved[2];
		}Ultra_Config;
};

FLASH_Status Flash_Write(uint32_t addr,uint32_t* buff,uint8_t len);
void Flash_Read(uint32_t addr,uint32_t *buff,uint8_t len);

#endif
