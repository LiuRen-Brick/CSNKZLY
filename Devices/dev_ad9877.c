/*
 * dev_ad9833.c
 *
 *  Created on: Mar 18, 2024
 *      Author: 15754
 */
#include "dev_ad9877.h"
#include "dev_gpio.h"

static void AD9833_Delay(void)
{
    uint16_t i;
    for (i = 0; i < 1; i++);
}

static void AD9833_FSYNC_Set(uint8_t ch,BitAction BitVal)
{
	DevGpio_SetOutPut(A_FSYNC, BitVal);
}

static void AD9833_SCLK_Set(uint8_t ch,BitAction BitVal)
{
	DevGpio_SetOutPut(A_SCLK, BitVal);
}

static void AD9833_SDATA_Set(uint8_t ch,BitAction BitVal)
{
	DevGpio_SetOutPut(A_SDATA, BitVal);
}

/**
 * @brief 向 AD9833 发送数据
 * 该函数通过指定的通道向 AD9833 数字频率合成器发送数据。发送的数据是一个 32 位的值。
 * @param ch 目标通道，用于指定数据发送的通道。
 * @param txdata 要发送的 32 位数据。
 * @return 该函数没有返回值。
 */
static void AD9833_SendData(uint8_t ch,uint32_t txdata)
{
	uint8_t tag_i = 0;

	AD9833_SCLK_Set(ch,Bit_SET);
	AD9833_FSYNC_Set(ch,Bit_SET);
	AD9833_FSYNC_Set(ch,Bit_RESET);

	for(tag_i=0;tag_i<16;tag_i++){
		if (txdata & 0x8000){
			AD9833_SDATA_Set(ch,Bit_SET);
		}
		else{
			AD9833_SDATA_Set(ch,Bit_RESET);
		}
		AD9833_Delay();
		AD9833_SCLK_Set(ch,Bit_RESET);
		AD9833_Delay();
		AD9833_SCLK_Set(ch,Bit_SET);
		txdata<<=1;
	}

	AD9833_FSYNC_Set(ch,Bit_SET);
}


static void AD9833_SendPhase(uint8_t ch,uint16_t reg, uint16_t val)
{
    uint16_t phase = reg;
    phase |= val;
    AD9833_SendData(ch,phase);
}

static void AD9833_SendWave(uint8_t ch,uint32_t WaveMode,uint32_t Freq_SFR,uint32_t Phase_SFR)
{
    uint32_t val = 0;
    val = (val | WaveMode | Freq_SFR | Phase_SFR);
    AD9833_SendData(ch,val);
}

static void AD9833_SendFrequency(uint8_t ch,unsigned short reg, double fout)
{
	int frequence_LSB,frequence_MSB;
	double   frequence_mid,frequence_DATA;
	long int frequence_hex;
	frequence_mid=268435456/11.059;
	frequence_DATA=fout;
	frequence_DATA=frequence_DATA/1000000;
	frequence_DATA=frequence_DATA*frequence_mid;
	frequence_hex=frequence_DATA;
	frequence_LSB=frequence_hex;
	frequence_LSB=frequence_LSB&0x3fff;
	frequence_MSB=frequence_hex>>14;
	frequence_MSB=frequence_MSB&0x3fff;
	frequence_LSB=frequence_LSB|reg;
	frequence_MSB=frequence_MSB|reg;
	AD9833_SendData(ch,0x2100);
	AD9833_SendData(ch,frequence_LSB);
	AD9833_SendData(ch,frequence_MSB);
}

void AD9833_SetPara(uint8_t ch,uint32_t Freq_SFR,double Freq,uint32_t Phase_SFR,uint32_t Phase,uint32_t WaveMode)
{
	unsigned int Fsel,Psel;
	AD9833_SendData(ch,0x0100);
	AD9833_SendData(ch,0x2100);
	AD9833_SendFrequency(ch,Freq_SFR,Freq);
	AD9833_SendPhase(ch,Phase_SFR,Phase);
	if(Freq_SFR == AD9833_REG_FREQ0)
	{
		Fsel = AD9833_FSEL0;
	}
	else
	{
		Fsel = AD9833_FSEL1;
	}
	if(Phase_SFR == AD9833_REG_PHASE0)
	{
		Psel = AD9833_PSEL0;
	}
	else
	{
		Psel = AD9833_PSEL1;
	}
	AD9833_SendWave(ch,WaveMode,Fsel,Psel);
}

/**
 * @brief 初始化 AD9833 的 I/O 引脚
 *
 * 该函数初始化指定通道的 AD9833 数字频率合成器的 I/O 引脚。
 *
 * @param ch 目标通道，用于指定要初始化的 AD9833 设备。
 *
 * @return 该函数没有返回值。
 */
void AD9833_InitIo(uint8_t ch)
{
	AD9833_FSYNC_Set(ch,Bit_SET);
	AD9833_SCLK_Set(ch,Bit_SET);
	AD9833_SDATA_Set(ch,Bit_SET);
}
