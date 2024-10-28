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
extern uint8_t PowerOff_Flg;
extern uint16_t CycleTime;
extern double setpoint;		// 设定温度
extern double current_temp; // 当前温度
extern union DATA_STORE Data_Store;
extern PIDController Pid_Contronl;
extern uint8_t UltraModule;

static void HighProTask(void *pvParameters);
static void MediumProTask(void *pvParamters);
static void LowProTask(void *pvParameters);

static xTaskHandle pInitTaskHandle = NULL;

static uint8_t Low_Battery_Flg = 0;
static uint8_t Low_Battery_old_Flg = 0;

uint32_t Frequnecy_Param = 0;
uint32_t Vibration_Param = 0;
uint8_t Led_RedFlg = 0;
uint8_t Led_GreenFlg = 0;
/*!
 *@brief:	 APP_Start
 *@param:	 none
 *@retval: none
 *@none:	 功能处理函数，根据模块需求进行分类
 */
void InitTask1(void *pvParameters)
{
		xTaskCreate(HighProTask, 
								(portCHAR const*) "HighProTask", 
								HIGHPROTASK_STACK_SIZE, 
								NULL, 
								TASK_PRIO_HIGH,
								NULL);
								
		xTaskCreate(MediumProTask, 
								(portCHAR const*) "MediumProTask", 
								HIGHPROTASK_STACK_SIZE, 
								NULL, 
								TASK_PRIO_HIGH,
								NULL);		
								
		xTaskCreate(LowProTask,
								(portCHAR const*) "LowProTask",
								LOWPROTASK_STACK_SIZE,
								NULL,
								TASK_PRIO_LOW,
								NULL);
								
		vTaskDelete(pInitTaskHandle);
}

static void HighProTask(void *pvParamters)
{
	for(;;)
	{
		Lipus_MainFunc();
		Power_MainFunc();
	}
}

static void MediumProTask(void *pvParamters)
{
	for(;;)
	{
		IWDG_ReloadCounter();
		ADC_MainFunc();
		Beep_MainFunc(Beep_SatrtFlg);
		vTaskDelay(200);
	}
}

static void LowProTask(void *pvParamters)
{
	for(;;)
	{
		StandyDete();
		//UltraParam_Set();
		LED_MainFunc();
		vTaskDelay(500);
	}
}

void APP_Start(void)
{
	xTaskCreate(InitTask1,
							 (portCHAR const* ) "vInitTask", 
							 128, 
							 NULL, 
							 5, 
							 &pInitTaskHandle);
}

// 初始化LED灯
void LED_Init(void)
{
	DevGpio_SetOutPut(LED_RED, Bit_SET);	   // 设置红色LED灯为关闭状态
	DevGpio_SetOutPut(LED_GREEN, Bit_RESET); // 设置绿色LED灯为开启状态
}

/*
 * 函数名称: LED_MainFunc
 * 功能描述: 控制LED的主要功能，根据充电标志设置LED的状态。
 * 参数: 无
 * 返回值: 无
 */
typedef enum
{
		LEDRed_On = 1,
		LEDRed_Glitter,
		LEDGreen_On,
		LEDGreen_Glitter
}LED_STATE;

void LED_MainFunc(void)
{
	//uint8_t alarmflg = 0;
	static uint8_t ledflg = LEDGreen_On;
	
	/*检测探头是否接入*/
	//	alarmflg = ProbTest_MainFunc();
	if(WorkStart_Flg == 1)
	{
			if((Battery_vol < 3300) || (Low_Battery_Flg == 1))
			{
					ledflg = LEDRed_Glitter;
				  Low_Battery_Flg = 1;
			}else if(Low_Battery_Flg == 0)
			{
					ledflg = LEDGreen_Glitter;
			}else
			{
			}
	}else
	{
			if((Battery_vol < 3200) || (Low_Battery_Flg == 1))
			{
					ledflg = LEDRed_On;
					Low_Battery_Flg = 1;
			}else if(Low_Battery_Flg == 0)
			{
					ledflg = LEDGreen_On;
			}else
			{
			}
	}
	if((WorkStart_Flg == 1) && (ledflg == Led_GreenFlg))
	{
			ledflg = LEDGreen_On;
	}else
	{
	}
	if(Battery_vol < 3100)
	{
			PowerOff_Flg = 1;
	}
		
	switch(ledflg)
	{
		case LEDRed_On:
			DevGpio_SetOutPut(LED_RED, Bit_RESET);
			DevGpio_SetOutPut(LED_GREEN, Bit_SET);   
		break;
		
		case LEDRed_Glitter:
			GPIO_ToggleBit(LED_RED);
			DevGpio_SetOutPut(LED_GREEN, Bit_SET);
		break;
		
		case LEDGreen_On:
			DevGpio_SetOutPut(LED_RED, Bit_SET);
			DevGpio_SetOutPut(LED_GREEN, Bit_RESET); 
		break;		
		
		case LEDGreen_Glitter:
			GPIO_ToggleBit(LED_GREEN);
			DevGpio_SetOutPut(LED_RED, Bit_SET);
		break;
		default:
			break;
	}
	
	if((Low_Battery_old_Flg != Low_Battery_Flg) && (Low_Battery_Flg == 1))
	{
			Beep_SatrtFlg = 5;
	}
	
}

static void Beep_MinorFunc(uint8_t count)
{
		static uint8_t Beep_SatrtFlg_old;
		static uint8_t BeepSta = 0;
		static uint8_t BeepCount = 0;
		
		if(Beep_SatrtFlg != Beep_SatrtFlg_old)
		{
			BeepSta = 0;
			BeepCount = 0;
			Beep_SatrtFlg_old = Beep_SatrtFlg;
		}
	
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

		if (BeepCount >= count)
		{
			Beep_SatrtFlg = 0;
			BeepCount = 0;
			Beep_SatrtFlg = 0;
			Low_Battery_old_Flg = Low_Battery_Flg;
		}else
		{
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
	static uint8_t BeepCount = 0;
	static uint8_t BeepSta = 0;
	static uint8_t Count = 0;
	static uint8_t Beep_SatrtFlg_old = 0;
	
	if(Beep_SatrtFlg != Beep_SatrtFlg_old)
	{
		Beep_SatrtFlg_old = 0;
		BeepSta = 0;
		BeepCount = 0;
		Beep_SatrtFlg_old = Beep_SatrtFlg;
	}
	
	switch (beep)
	{
		case 1:
			
			Beep_MinorFunc(1);
		break;
		
		case 2:
			
			Beep_MinorFunc(UltraModule);
		break;
		
		case 3:
			
			Beep_MinorFunc(Motor_Level);
		break;

		case 4:
			Count++;
			
			if((Count <= 4) || ((Count > 8) && (Count <= 12)))
			{
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
		  }else
			{
			}

			if(BeepCount >= 4)
			{
				Count = 0;
				Beep_SatrtFlg = 0;
				BeepCount = 0;
				Beep_SatrtFlg = 0;
			}else
			{
			}
			break;
		case 5:
			if((Low_Battery_Flg != Low_Battery_old_Flg) && (Low_Battery_Flg == 1))
			{
				Beep_MinorFunc(3);
			}else
			{
				Beep_SatrtFlg = 0;
				BeepCount = 0;
				Beep_SatrtFlg = 0;
			}
		break;
			
		default:
			Count = 0;
			Beep_SatrtFlg = 0;
			BeepCount = 0;
		break;
	}
}
/*
 * 函数名称: Motor_LevelSet
 * 功能描述: 根据Motor_Level的值设置电机的震动强度。
 * 参数: 无
 * 返回值: 设置的占空比值
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

/*!
 *@brief : Motor_MainFunc
 *@param : 无
 *@retval: 无
 *@none  : 
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
 *@breif :  Lipus_MainFunc
 *@param :  none
 *@retval:  none
 *@note  :  控制脉冲信号的发出，加入PID温度调控功能
 */
/*用于超声相关输出的功能函数*/
void Lipus_MainFunc(void)
{
	// 静态变量，用于存储上一次的工作启动标志状态
	// static uint8_t WorkStart_Flg_old = 0;
	static uint8_t CycleDuty = 100;
	float current_temp = 0;
	uint8_t OverTemp_Flg = 0;
	uint8_t duty = 100;
	
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
			//DevGpio_SetOutPut(MOTOR_GATE, Bit_SET);
			Devpwm_SetDuty(SET_PWM1, duty);
		}else
		{
			// 设置相关GPIO输出为低电平
			DevGpio_SetOutPut(V12_EN, Bit_RESET);
			DevGpio_SetOutPut(V45_EN, Bit_RESET);
			DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
			//DevGpio_SetOutPut(MOTOR_GATE, Bit_RESET);
			// 设置PWM占空比为0%
			Devpwm_SetDuty(SET_PWM1, 0);
			CycleTime = 0;
		}
		// 更新上一次的工作启动标志状态
		// WorkStart_Flg_old = WorkStart_Flg;
	}
}

/*!
 *@biref : Power_MainFunc
 *@param : none
 *@retval: none
 *@note  : 常按关机功能，常按2S以上拉低PA9电压从而实现关机功能，常按期间其余所有功能失效
 */

void Power_MainFunc(void)
{
	if (PowerOff_Flg == 1)
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
		GPIO_DeInit(GPIOA);
		GPIO_DeInit(GPIOB);
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM15, DISABLE);
		TIM_Cmd(TIM16, DISABLE);

		while (PowerOff_Flg)
			POWER_OFF;
	}
}

/*!
 *@brief : ProbTest_MainFunc
 *@param : none
 *@retval: none
 *@none  : 检测探头是否接入
 */
uint8_t ProbTest_MainFunc(void)
{
		uint8_t probflg = 0;
		
		probflg = DevGpio_ReadInPut(MONIT);
	
		return probflg;
}

/*!
 *@brief :StandyDete
 *@param :none
 *@retval:none
 *@none  :待机检测功能，非工作状态持续45S自动关机
 */

void StandyDete(void)
{
		static uint32_t StandyCount = 0;
	
		if(WorkStart_Flg == 1)
		{
				StandyCount = 0;
		}else
		{
			  StandyCount++;
		}
		
		if(StandyCount > 90)
		{
				PowerOff_Flg = 1;
		}
}

