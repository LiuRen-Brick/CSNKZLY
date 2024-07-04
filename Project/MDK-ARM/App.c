#include "App.h"
#include "dev_adc.h"
#include "dev_gpio.h"
#include "dev_pwm.h"

extern float Battery_vol;
extern uint16_t GREEN_Count;
extern uint32_t Beep_Count;
extern uint8_t WorkStart_Flg;
extern uint8_t Beep_SatrtFlg;
extern uint8_t Chrg_Flg;
extern uint8_t Stdby_Flg;
uint8_t Led_RedFlg = 0;
uint8_t Led_GreenFlg = 0;

void APP_Start(void)
{
	ADC_MainFunc();
	GPIO_ReadMainFunc();
	LED_MainFunc();
	Lipus_MainFunc();
	Beep_MainFunc();
}

void LED_Init(void)
{
	DevGpio_SetOutPut(LED_RED,Bit_SET);
	DevGpio_SetOutPut(LED_GREEN,Bit_RESET);
}

/*
 *
 *
 *
 */

void LED_MainFunc(void)
{
		if(Chrg_Flg == 0x01)// || (Battery_vol < 3200))
		{
				Led_RedFlg = 1;
		}else{
				Led_RedFlg = 0;
		}
		
		if(WorkStart_Flg == 0x01)
		{
				Led_GreenFlg = 0x02;
		}else if(Led_RedFlg == 0x01)
		{
				Led_GreenFlg = 0;
		}else
		{
				Led_GreenFlg = 0x01;
		}
		
		if(Led_RedFlg == 0x01)
		{
				DevGpio_SetOutPut(LED_RED,Bit_RESET);
		}else
		{
				DevGpio_SetOutPut(LED_RED,Bit_SET);
		}
		
		if(Led_GreenFlg == 0x02)
		{
			  if(GREEN_Count > 500)
				{
						GPIO_ToggleBit(LED_GREEN);
					  GREEN_Count = 0;
				}
		}else if(Led_GreenFlg == 0x01)
		{
			 DevGpio_SetOutPut(LED_GREEN,Bit_RESET);
		}else
		{
			 DevGpio_SetOutPut(LED_GREEN,Bit_SET);
		}
}

static void Beep_ON(void)
{
		Devpwm_SetDuty(SET_PWM2,50);
}

static void Beep_OFF(void)
{
		Devpwm_SetDuty(SET_PWM2,0);
}

void Beep_MainFunc(void)
{
		static uint8_t BeepStart = 0;
	  static uint8_t BeepCount = 0;
		if(Beep_SatrtFlg == 1)
		{
				if((Beep_Count > 20) && (BeepStart == 0))
				{
						Beep_OFF();
						BeepStart = 1;
					  Beep_Count = 0;
					  BeepCount++;
				}else if((Beep_Count > 20) && (BeepStart == 1))
				{
						Beep_ON();
						BeepStart = 0;
					  Beep_Count = 0;
					  BeepCount++;
				}
				
				if(BeepCount > 4)
				{
					  BeepStart = 0;
					  BeepCount = 0;
						Beep_SatrtFlg = 0;
				}
		}else
		{
				Beep_Count = 0;
		}
}
/*
 *
 *
 *
 */
void Power_Init(void)
{

}

void Lipus_MainFunc(void)
{
	static uint8_t WorkStart_Flg_old = 0;

	if((WorkStart_Flg_old != WorkStart_Flg) && (WorkStart_Flg == 1))
	{

		DevGpio_SetOutPut(V12_EN,Bit_SET);
		DevGpio_SetOutPut(V45_EN,Bit_SET);
	  DevGpio_SetOutPut(WAVE_EN,Bit_SET);
		Devpwm_SetDuty(SET_PWM3,50);
		Devpwm_SetDuty(SET_PWM1,75);

		WorkStart_Flg_old = WorkStart_Flg;
	}else if((WorkStart_Flg_old != WorkStart_Flg) && (WorkStart_Flg == 0)){
		DevGpio_SetOutPut(V12_EN,Bit_RESET);
		DevGpio_SetOutPut(V45_EN,Bit_RESET);
		DevGpio_SetOutPut(WAVE_EN,Bit_RESET);
		Devpwm_SetDuty(SET_PWM3,0);
		Devpwm_SetDuty(SET_PWM1,0);

		WorkStart_Flg_old = WorkStart_Flg;
	}
}


