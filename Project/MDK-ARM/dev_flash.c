#include "dev_flash.h"

union DATA_STORE Data_Store;
/*
 * 函数: Flash_Write
 * 功能: 将数据写入指定地址的Flash存储器
 * 参数:
 *   addr - 写入的起始地址
 *   buff - 指向要写入数据的缓冲区
 *   len - 要写入的数据长度
 * 返回:
 *   FLASH_Status - 写入操作的状态
 * 
 */
FLASH_Status Flash_Write(uint32_t addr, uint32_t* buff, uint8_t len)
{
  uint8_t tag_i = 0;
	uint32_t writedata = 0;
  uint32_t writeaddr = 0;
	FLASH_Status status = FLASH_BUSY;
  FLASH_Unlock();
  
  writeaddr = addr;
  status = FLASH_ErasePage(writeaddr);
  if(status != FLASH_COMPLETE)
  {
    return status;
  }
   
  for(tag_i = 0; tag_i < (len/2 + 1); tag_i++)
  {
		writedata = *(&buff+tag_i);
		status = FLASH_ProgramWord(writeaddr, writedata);
    if(status == FLASH_COMPLETE)
    {
      writeaddr += 4;
    }else
    {
      break;
    }
  }
  FLASH_Lock();
  return status;
}

/**
 * @brief 从指定地址的闪存中读取数据
 * 该函数从指定地址的闪存中读取数据，并将数据存储到缓冲区中。读取操作的长度由 `len` 参数指定。
 * @param addr 目标闪存的起始地址，必须是四字节，以确保对齐。
 * @param buff 指向存储读取数据的缓冲区的指针。
 * @param len 要读取的数据长度，以 32 位字为单位。
 * @return 该函数没有返回值。
 */
void Flash_Read(uint32_t addr, uint32_t *buff, uint8_t len)
{
    uint8_t tag_i = 0;
    uint32_t readaddr = 0;

    // 解锁闪存
    FLASH_Unlock();
    readaddr = addr;
    for (tag_i = 0; tag_i < len; tag_i++)
    {
        *(buff + tag_i) = *(volatile uint32_t *)readaddr;
        readaddr += 4;
    }
    // 锁定闪存
    FLASH_Lock();
}

