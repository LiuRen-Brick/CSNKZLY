#ifndef __APP_H
#define __APP_H

#include "main.h"

typedef enum
{
	IDLE_STATE,
	CHARGE_STATE,
	PAUSE_STATE,
	WORK_STATE,
  CLOSE_STATE,
}DEV_WORK_STATE;

void APP_Start(void);
void LED_Init(void);
void LED_MainFunc(void);
void Beep_MainFunc(uint8_t beep);
void Motor_MainFunc(void);
void Lipus_MainFunc(void);
void Get_ChargeSta(void);
uint8_t ProbTest_MainFunc(void);
void InitTask1(void *pvParameters);
void DevLed_MainFunc(void);

void Power_MainFunc(void);

#endif
