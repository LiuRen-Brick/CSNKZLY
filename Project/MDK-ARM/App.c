#include "App.h"
#include "dev_adc.h"
#include "dev_gpio.h"
#include "dev_pwm.h"

extern float Battery_vol;
extern uint16_t GREEN_Count;
extern uint32_t Beep_Count;
extern uint8_t WorkStart_Flg;
extern uint8_t Beep_SatrtFlg;
extern uint8_t Stdby_Flg;
extern uint8_t Motor_Level;

uint8_t Led_RedFlg = 0;
uint8_t Led_GreenFlg = 0;

void APP_Start(void)
{
	ADC_MainFunc();
	GPIO_ReadMainFunc();
	LED_MainFunc();
	Lipus_MainFunc();
	Motor_MainFunc();
	Beep_MainFunc();
}

// 初始化LED灯
void LED_Init(void)
{
 DevGpio_SetOutPut(LED_RED,Bit_SET); // 设置红色LED灯为关闭状态
 DevGpio_SetOutPut(LED_GREEN,Bit_RESET); // 设置绿色LED灯为开启状态
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
	if(Battery_vol < 3200)
	{
		Led_RedFlg = 1;
	}else{
		Led_RedFlg = 0;
	}
	
	// 根据工作开始标志和红色LED标志设置绿色LED标志
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
	
	// 根据红色LED标志设置红色LED输出
	if(Led_RedFlg == 0x01)
	{
		DevGpio_SetOutPut(LED_RED,Bit_RESET);
	}else
	{
		DevGpio_SetOutPut(LED_RED,Bit_SET);
	}
	
	// 根据绿色LED标志设置绿色LED输出
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

// 使蜂鸣器开启的函数
static void Beep_ON(void)
{
	Devpwm_SetDuty(SET_PWM2,50); // 设置PWM2的占空比为50%
}

// 关闭蜂鸣器
static void Beep_OFF(void)
{
	Devpwm_SetDuty(SET_PWM2,0); // 设置PWM2的占空比为0，关闭蜂鸣器
}

/*
 * 函数名称: Beep_MainFunc
 * 功能描述: 用于控制蜂鸣器的启动和关闭。
 * 参数: 无
 * 返回值: 无
 */
void Beep_MainFunc(void)
{
    // 静态变量，用于控制蜂鸣器的启动状态
    static uint8_t BeepStart = 0;
    // 静态变量，用于记录蜂鸣器的计数
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
 * 函数名称: Motor_LevelSet
 * 功能描述: 根据Motor_Level的值设置电机的震动强度。
 * 参数: 无
 * 返回值: uint8_t - 设置的占空比值
 */
uint8_t Motor_LevelSet(void)
{
	uint8_t duty = 0;

	if(Motor_Level == 0)
	{
		duty = 100;
	}else if(Motor_Level == 1)
	{
		duty  = 85;
	}else if(Motor_Level == 2)
	{
		duty = 50;
	}

	return duty;
}

// 电机主函数，用于控制电机的工作状态
void Motor_MainFunc(void)
{
	static uint8_t motor_duty_old = 0; // 保存上一次的电机占空比
	uint8_t motor_duty = 0; // 当前电机占空比

	motor_duty = Motor_LevelSet(); // 获取当前电机占空比

	// 如果当前电机占空比与上一次不同，则更新PWM占空比
	if(motor_duty != motor_duty_old)
	{
		Devpwm_SetDuty(SET_PWM3,motor_duty); // 设置PWM占空比
		motor_duty_old = motor_duty; // 更新上一次的电机占空比
	}
}

/*用于超声相关输出的功能函数*/
void Lipus_MainFunc(void)
{
	// 静态变量，用于存储上一次的工作启动标志状态
	static uint8_t WorkStart_Flg_old = 0;

	// 检查工作启动标志是否发生变化且当前标志为启动状态
	if(WorkStart_Flg_old != WorkStart_Flg)
	{
		if(WorkStart_Flg == 1)
		{
			// 设置相关GPIO输出为高电平
			DevGpio_SetOutPut(V12_EN,Bit_SET);
			DevGpio_SetOutPut(V45_EN,Bit_SET);
			DevGpio_SetOutPut(WAVE_EN,Bit_SET);
			DevGpio_SetOutPut(MOTOR_GATE,Bit_SET);
			// 设置PWM占空比为100%
			Devpwm_SetDuty(SET_PWM1,100);
		}else
		{
			// 设置相关GPIO输出为低电平
			DevGpio_SetOutPut(V12_EN,Bit_RESET);
			DevGpio_SetOutPut(V45_EN,Bit_RESET);
			DevGpio_SetOutPut(WAVE_EN,Bit_RESET);
			DevGpio_SetOutPut(MOTOR_GATE,Bit_RESET);
			// 设置PWM占空比为0%
			Devpwm_SetDuty(SET_PWM1,0);
		}
		// 更新上一次的工作启动标志状态
		WorkStart_Flg_old = WorkStart_Flg;
	}
}


