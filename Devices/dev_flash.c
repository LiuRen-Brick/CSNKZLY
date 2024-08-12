#include "dev_flash.h"
#include "dev_ad9877.h"
#include "dev_pwm.h"

union DATA_STORE Data_Store;
extern uint8_t SetFreq_Flg;
extern uint8_t Clear_Flg;
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
  
  if (addr % 4 != 0) {
    return FLASH_ERROR_PROGRAM; // 假设有一个表示对齐错误的错误码
  }
  
  FLASH_Unlock();
  
  writeaddr = addr;
  status = FLASH_ErasePage(writeaddr);
  if(status != FLASH_COMPLETE)
  {
		FLASH_Lock();
    return status;
  }
   
  for(tag_i = 0; tag_i < len; tag_i++)
  {
		writedata = buff[tag_i];
		status = FLASH_ProgramWord(writeaddr, writedata);
    if(status == FLASH_COMPLETE)
    {
				break;
    }
		writeaddr += 4;
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

void UltraParam_Init(void)
{
		uint32_t fre = 0;

		Flash_Read(FLASHSTORE,&Data_Store.data,1);
	
		if((Data_Store.Ultra_Config.Frequency == 0xFF) && (Data_Store.Ultra_Config.Vibration == 0xFF))
		{
				Data_Store.Ultra_Config.Frequency = 25;
				Data_Store.Ultra_Config.Vibration = 5;
				Flash_Write(FLASHSTORE,&Data_Store.data,1);
		}
		
		fre = 1900000 + 4000 * Data_Store.Ultra_Config.Frequency;
		Devpwm_SetDuty(SET_PWM3,100);
		AD9833_InitIo(AD9877_Ch_A);
		AD9833_SetPara(AD9877_Ch_A,AD9833_REG_FREQ0,fre,AD9833_REG_PHASE1,2048,AD9833_OUT_TRIANGLE);
}

void UltraParam_Set(void)
{
		static uint8_t SetFreq_Flg_old = 0;
    static uint8_t Clear_Flg_old = 0;
		uint32_t fre = 0;
	
	  if((SetFreq_Flg_old != SetFreq_Flg) && (SetFreq_Flg == 1))
		{
				Data_Store.Ultra_Config.Frequency++;
				fre = 1900000 + 4000 * Data_Store.Ultra_Config.Frequency;
			  AD9833_SetPara(AD9877_Ch_A,AD9833_REG_FREQ0,fre,AD9833_REG_PHASE1,2048,AD9833_OUT_TRIANGLE);
			  Flash_Write(FLASHSTORE,&Data_Store.data,1);
		}else{}
			
		if((Clear_Flg_old != Clear_Flg) && (Clear_Flg == 1))
		{
				Data_Store.Ultra_Config.Frequency = 0;
				fre = 1900000 + 4000 * Data_Store.Ultra_Config.Frequency;
			  AD9833_SetPara(AD9877_Ch_A,AD9833_REG_FREQ0,fre,AD9833_REG_PHASE1,2048,AD9833_OUT_TRIANGLE);
			  Flash_Write(FLASHSTORE,&Data_Store.data,1);
		}else{}
			
		SetFreq_Flg_old = SetFreq_Flg;
		Clear_Flg_old = Clear_Flg;
}

