#ifndef __APP_H
#define __APP_H

#include "main.h"

void APP_Start(void);
void LED_Init(void);
void LED_MainFunc(void);
void Beep_MainFunc(void);
void Motor_MainFunc(void);
void Lipus_MainFunc(void);
void Get_ChargeSta(void);
uint8_t Motor_LevelSet(void);

void Power_MainFunc(void);

#endif
