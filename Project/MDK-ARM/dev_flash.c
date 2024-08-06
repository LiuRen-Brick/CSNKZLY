#include "dev_flash.h"

union DATA_STORE Data_Store;
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

