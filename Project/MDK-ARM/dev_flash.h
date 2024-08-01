#ifndef __DEV_FLASH_H
#define __DEV_FLASH_H 

#include "main.h"

#define FLASHSTORE 0x0800F000

typedef union
{
		uint32_t store_data;
		uint8_t store_buff[4];
}ULTRA_CONFIG;

FLASH_Status Flash_Write(uint32_t addr,uint32_t* buff,uint8_t len);
void Flash_Read(uint32_t addr,uint32_t *buff,uint8_t len);

#endif
