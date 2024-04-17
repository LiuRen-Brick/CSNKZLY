#ifndef __DEV_GPIO_H
#define __DEV_GPIO_H

#include "main.h"

typedef enum
{
	LED_RED,
	LED_GREEN,
	MOTOR_GATE,
	V45_EN,
	V12_EN,
	WAVE_EN,
	A_SCLK,
	A_SDATA,
	A_FSYNC,
	DevGpioMaxChannlLab,
}DEVGpioChannlLabType;

void DevGpio_SetOutPut(uint8_t ch,BitAction BitVal);

#endif
