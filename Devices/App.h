#ifndef __APP_H
#define __APP_H

#include "main.h"

void APP_Start(void);
void LED_Init(void);
void LED_MainFunc(void);
void Beep_MainFunc(uint8_t beep);
void Motor_MainFunc(void);
void Lipus_MainFunc(void);
void Get_ChargeSta(void);
uint8_t Motor_LevelSet(uint8_t level);

void Power_MainFunc(void);

#endif
