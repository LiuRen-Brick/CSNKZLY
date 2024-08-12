#include "dev_flash.h"
#include "dev_ad9877.h"
#include "dev_pwm.h"

union DATA_STORE Data_Store;
extern uint8_t SetFreq_Flg;
extern uint8_t Clear_Flg;
/*
 * ����: Flash_Write
 * ����: ������д��ָ����ַ��Flash�洢��
 * ����:
 *   addr - д�����ʼ��ַ
 *   buff - ָ��Ҫд�����ݵĻ�����
 *   len - Ҫд������ݳ���
 * ����:
 *   FLASH_Status - д�������״̬
 * 
 */
FLASH_Status Flash_Write(uint32_t addr, uint32_t* buff, uint8_t len)
{
  uint8_t tag_i = 0;
	uint32_t writedata = 0;
  uint32_t writeaddr = 0;
	FLASH_Status status = FLASH_BUSY;
  
  if (addr % 4 != 0) {
    return FLASH_ERROR_PROGRAM; // ������һ����ʾ�������Ĵ�����
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
 * @brief ��ָ����ַ�������ж�ȡ����
 * �ú�����ָ����ַ�������ж�ȡ���ݣ��������ݴ洢���������С���ȡ�����ĳ����� `len` ����ָ����
 * @param addr Ŀ���������ʼ��ַ�����������ֽڣ���ȷ�����롣
 * @param buff ָ��洢��ȡ���ݵĻ�������ָ�롣
 * @param len Ҫ��ȡ�����ݳ��ȣ��� 32 λ��Ϊ��λ��
 * @return �ú���û�з���ֵ��
 */
void Flash_Read(uint32_t addr, uint32_t *buff, uint8_t len)
{
    uint8_t tag_i = 0;
    uint32_t readaddr = 0;

    // ��������
    FLASH_Unlock();
    readaddr = addr;
    for (tag_i = 0; tag_i < len; tag_i++)
    {
        *(buff + tag_i) = *(volatile uint32_t *)readaddr;
        readaddr += 4;
    }
    // ��������
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

