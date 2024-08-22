#ifndef __DEV_GPIO_H
#define __DEV_GPIO_H

#include "main.h"

typedef enum
{
	LED_RED,				//��ɫLED
	LED_GREEN, 			//��ɫLED
	MOTOR_GATE, 		//��￪��
	V45_EN,					//45V ����
	V12_EN,					//12V ����
	WAVE_EN,				//���� ����
	A_SCLK,					//AD9833 ʱ����
	A_SDATA,				//AD9833 ������
	A_FSYNC,        //AD9833 ʹ����
	POWER,					//��Դ����
	SWITCH,					//
	MONIT,
	DevGpioMaxChannlLab,
}DEVGpioChannlLabType;

void DevGpio_SetOutPut(uint8_t ch,BitAction BitVal);
void GPIO_ToggleBit(uint8_t ch);
uint8_t DevGpio_ReadInPut(uint8_t ch);

#endif
