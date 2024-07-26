#ifndef __DEV_FLASH_H
#define __DEV_FLASH_H 

#include "main.h"

FLASH_Status Flash_Write(uint32_t addr,uint16_t* buff,uint8_t len);
void Flash_Read(uint32_t addr,uint16_t *buff,uint8_t len);

#endif
