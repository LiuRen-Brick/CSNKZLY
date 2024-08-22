#ifndef __DEV_GPIO_H
#define __DEV_GPIO_H

#include "main.h"

typedef enum
{
	LED_RED,				//红色LED
	LED_GREEN, 			//绿色LED
	MOTOR_GATE, 		//马达开关
	V45_EN,					//45V 开关
	V12_EN,					//12V 开关
	WAVE_EN,				//声波 开关
	A_SCLK,					//AD9833 时钟线
	A_SDATA,				//AD9833 数据线
	A_FSYNC,        //AD9833 使能线
	POWER,					//电源开关
	SWITCH,					//
	MONIT,
	DevGpioMaxChannlLab,
}DEVGpioChannlLabType;

void DevGpio_SetOutPut(uint8_t ch,BitAction BitVal);
void GPIO_ToggleBit(uint8_t ch);
uint8_t DevGpio_ReadInPut(uint8_t ch);

#endif
