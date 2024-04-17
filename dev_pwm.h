#ifndef __DEV_PWM_H
#define __DEV_PWM_H

#include "main.h"

typedef enum
{
	SET_PWM1,
	SET_PWM14,
	SET_PWM_MAX_NUM,
}PWMSETTABLE;

void Devpwm_SetDuty(uint8_t channle,uint16_t duty);
	
#endif
