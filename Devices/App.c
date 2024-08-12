#include "App.h"
#include "dev_adc.h"
#include "dev_gpio.h"
#include "dev_pwm.h"
#include "dev_flash.h"
#include "dev_pid.h"

#define BEEP_ON Devpwm_SetDuty(SET_PWM2, 15)
#define BEEP_OFF Devpwm_SetDuty(SET_PWM2, 0)

extern float Battery_vol;
extern float Ultra_Temp;
extern uint16_t GREEN_Count;
extern uint8_t WorkStart_Flg;
extern uint8_t Beep_SatrtFlg;
extern uint8_t Stdby_Flg;
extern uint8_t Motor_Level;
extern uint8_t Power_Flg;
extern uint16_t CycleTime;
extern double setpoint;		// 设定温度
extern double current_temp; // 当前温度
extern union DATA_STORE Data_Store;
extern PIDController Pid_Contronl;
extern uint8_t UltraModule;

uint32_t Frequnecy_Param = 0;
uint32_t Vibration_Param = 0;
uint8_t Led_RedFlg = 0;
uint8_t Led_GreenFlg = 0;

void APP_Start(void)
{
	if (Task10msFlg)
	{
		ADC_MainFunc();
		Task10msFlg = FALSE;
	}

	if (Task20msFlg)
	{
		Task20msFlg = FALSE;
	}

	if (Task50msFlg)
	{
		Motor_MainFunc();
		Task50msFlg = FALSE;
	}

	if (Task100msFlg)
	{
		UltraParam_Set();
		Task100msFlg = FALSE;
	}

	if (Task200msFlg)
	{
		//IWDG_ReloadCounter();
		Beep_MainFunc(Beep_SatrtFlg);
		Task200msFlg = FALSE;
	}

	if (Task500msFlg)
	{
		// IWDG_ReloadCounter();
		Task500msFlg = FALSE;
	}

	if (Task1000msFlg)
	{
		LED_MainFunc();
		Task1000msFlg = FALSE;
	}
	Lipus_MainFunc();
	Power_MainFunc();
}

// 初始化LED灯
void LED_Init(void)
{
	DevGpio_SetOutPut(LED_RED, Bit_SET);	 // 设置红色LED灯为关闭状态
	DevGpio_SetOutPut(LED_GREEN, Bit_RESET); // 设置绿色LED灯为开启状态
}

/*
 * 函数名称: LED_MainFunc
 * 功能描述: 控制LED的主要功能，根据充电标志设置LED的状态。
 * 参数: 无
 * 返回值: 无
 */
void LED_MainFunc(void)
{
	// 根据充电标志设置红色LED标志
	if (Battery_vol < 3200)
	{
		Led_RedFlg = 1;
	}
	else
	{
		Led_RedFlg = 0;
	}

	// 根据工作开始标志和红色LED标志设置绿色LED标志
	if (WorkStart_Flg == 0x01)
	{
		Led_GreenFlg = 0x02;
	}
	else if (Led_RedFlg == 0x01)
	{
		Led_GreenFlg = 0;
	}
	else
	{
		Led_GreenFlg = 0x01;
	}

	// 根据红色LED标志设置红色LED输出
	if (Led_RedFlg == 0x01)
	{
		DevGpio_SetOutPut(LED_RED, Bit_RESET);
	}
	else
	{
		DevGpio_SetOutPut(LED_RED, Bit_SET);
	}

	// 根据绿色LED标志设置绿色LED输出
	if (Led_GreenFlg == 0x02)
	{
		GPIO_ToggleBit(LED_GREEN);
	}
	else if (Led_GreenFlg == 0x01)
	{
		DevGpio_SetOutPut(LED_GREEN, Bit_RESET);
	}
	else
	{
		DevGpio_SetOutPut(LED_GREEN, Bit_SET);
	}
}

/*
 * 函数名称: Beep_MainFunc
 * 功能描述: 用于控制蜂鸣器的启动和关闭。
 * 参数: 无
 * 返回值: 无
 */
void Beep_MainFunc(uint8_t beep)
{
	// 静态变量，用于记录蜂鸣器的计数
	static uint8_t BeepCount = 0;
	static uint8_t BeepSta = 0;

	switch (beep)
	{
		case 1:
			if (BeepSta == 0)
			{
				BEEP_ON;
				BeepSta = 1;
			}else if (BeepSta == 1)
			{
				BEEP_OFF;
				BeepSta = 0;
				BeepCount++;
			}

			if (BeepCount >= 1)
			{
				Beep_SatrtFlg = 0;
				BeepCount = 0;
			}else
			{
			}
					
		break;
		case 2:

			if (BeepSta == 0)
			{
				BEEP_ON;
				BeepSta = 1;
			}else if (BeepSta == 1)
			{
				BEEP_OFF;
				BeepSta = 0;
				BeepCount++;
			}

			if (BeepCount >= UltraModule)
			{
				Beep_SatrtFlg = 0;
				BeepCount = 0;
			}else
			{
			}
		break;
		case 3:
			if (BeepSta == 0)
			{
				BEEP_ON;
				BeepSta = 1;
			}else if (BeepSta == 1)
			{
				BEEP_OFF;
				BeepSta = 0;
				BeepCount++;
			}

			if (BeepCount >= Motor_Level)
			{
				Beep_SatrtFlg = 0;
				BeepCount = 0;
			}else
			{
			}
		break;

		case 4:
			if (BeepSta == 0)
			{
				BEEP_ON;
				BeepSta = 1;
			}else if (BeepSta == 1)
			{
				BEEP_OFF;
				BeepSta = 0;
				BeepCount++;
			}

			if (BeepCount >= 2)
			{
				Beep_SatrtFlg = 0;
				BeepCount = 0;
			}else
			{
			}
			break;
		default:
		break;
	}
}
/*
 * 函数名称: Motor_LevelSet
 * 功能描述: 根据Motor_Level的值设置电机的震动强度。
 * 参数: 无
 * 返回值: uint8_t - 设置的占空比值
 */
static uint8_t Motor_LevelSet(uint8_t level)
{
	uint8_t duty = 0;

	switch (level)
	{
		case 1:
			duty = 100;
		break;
		case 2:
			duty = 92;
		break;
		case 3:
			duty = 80;
		break;
		
		case 4:
			duty = 65;
		break;
		case 5:
			duty = 50;
		break;
		default:
		break;
	}

	return duty;
}

/*
 * 函数名称: Motor_MainFunc
 * 参    数: 无
 * 返 回 值: 无
 * 功能描述: 震动调整功能，通过按键调整电机震动等级，
 *           目前只有三个等级，可在超声工作时修改;
 */
void Motor_MainFunc(void)
{
	static uint8_t motor_duty_old = 0; // 保存上一次的电机占空比
	uint8_t motor_duty = 0;			   // 当前电机占空比

	motor_duty = Motor_LevelSet(Motor_Level); // 获取当前电机占空比

	// 如果当前电机占空比与上一次不同，则更新PWM占空比
	if (motor_duty != motor_duty_old)
	{
		Devpwm_SetDuty(SET_PWM3, motor_duty); // 设置PWM占空比
		motor_duty_old = motor_duty;		  // 更新上一次的电机占空比
	}
}

/*!
 *@breif:  超声驱动函数
 *@param:  none
 *@retval: none
 *@note:   控制脉冲信号的发出，加入PID温度调控功能
 */
/*用于超声相关输出的功能函数*/
void Lipus_MainFunc(void)
{
	// 静态变量，用于存储上一次的工作启动标志状态
	// static uint8_t WorkStart_Flg_old = 0;
	static uint8_t CycleDuty = 100;
	float current_temp = 0;
	uint8_t OverTemp_Flg = 0;
	uint8_t duty = 0;
	
	if(UltraModule == 1)
	{
			duty = 25;
	}else if(UltraModule == 2)
	{
			duty = 66;
	}

	// 检查工作启动标志是否发生变化且当前标志为启动状态
	// if(WorkStart_Flg_old != WorkStart_Flg)
	{
		if (WorkStart_Flg == 1)
		{
			// 设置相关GPIO输出为高电平
			DevGpio_SetOutPut(V12_EN, Bit_SET);
			DevGpio_SetOutPut(V45_EN, Bit_SET);
			DevGpio_SetOutPut(WAVE_EN, Bit_SET);
			DevGpio_SetOutPut(MOTOR_GATE, Bit_SET);
			// 设置超声脉冲占空比为66%
			current_temp = Ultra_Temp;
			CycleDuty = (uint8_t)PIDController_Update(&Pid_Contronl, current_temp, 1);
			if (current_temp > 38.0)
			{
				if (CycleTime < (CycleDuty * 20))
				{
					Devpwm_SetDuty(SET_PWM1, duty);
				}
				else if ((CycleTime >= (CycleDuty * 20)) && (CycleTime < 2000))
				{
					Devpwm_SetDuty(SET_PWM1, 00);
				}
				else
				{
					CycleTime = 0;
				}
				OverTemp_Flg = 1;
			}
			else if (current_temp < 35.0)
			{
				Devpwm_SetDuty(SET_PWM1, duty);
				OverTemp_Flg = 0;
			}
			else
			{
				if (OverTemp_Flg == 1)
				{
					if (CycleTime < (CycleDuty * 20))
					{
						Devpwm_SetDuty(SET_PWM1, duty);
					}
					else if ((CycleTime >= (CycleDuty * 20)) && (CycleTime < 2000))
					{
						Devpwm_SetDuty(SET_PWM1, 00);
					}
					else
					{
						CycleTime = 0;
					}
				}
				else
				{
					Devpwm_SetDuty(SET_PWM1, duty);
					OverTemp_Flg = 0;
				}
			}
		}
		else
		{
			// 设置相关GPIO输出为低电平
			DevGpio_SetOutPut(V12_EN, Bit_RESET);
			DevGpio_SetOutPut(V45_EN, Bit_RESET);
			DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
			DevGpio_SetOutPut(MOTOR_GATE, Bit_RESET);
			// 设置PWM占空比为0%
			Devpwm_SetDuty(SET_PWM1, 0);
			CycleTime = 0;
		}
		// 更新上一次的工作启动标志状态
		// WorkStart_Flg_old = WorkStart_Flg;
	}
}

/*
 * 函数名称: Power_MainFunc
 * 功能描述: 常按关机功能，常按2S以上拉低PA9电压从而实现关机功能，常按期间其余所有功能失效
 * 参    数: 无
 * 返 回 值: 无
 */

void Power_MainFunc(void)
{
	if (Power_Flg == 1)
	{
		POWER_OFF;
		DevGpio_SetOutPut(LED_RED, Bit_SET);   // 设置红色LED灯为关闭状态
		DevGpio_SetOutPut(LED_GREEN, Bit_SET); // 设置绿色LED灯为关闭状态
		// 设置相关GPIO输出为低电平
		DevGpio_SetOutPut(V12_EN, Bit_RESET);
		DevGpio_SetOutPut(V45_EN, Bit_RESET);
		DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
		DevGpio_SetOutPut(MOTOR_GATE, Bit_RESET);
		// 设置PWM占空比为0%
		Devpwm_SetDuty(SET_PWM1, 0);
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM15, DISABLE);
		TIM_Cmd(TIM16, DISABLE);

		while (Power_Flg)
			;
	}
}
