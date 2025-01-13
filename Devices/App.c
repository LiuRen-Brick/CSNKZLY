#include "App.h"
#include "dev_adc.h"
#include "dev_gpio.h"
#include "dev_pwm.h"
#include "dev_flash.h"
#include "dev_beep.h"

extern float Battery_vol;
extern uint8_t PowerOff_Flg;
extern uint32_t StandyTime;

DEV_WORK_STATE DevWorkState = IDLE_STATE;
uint8_t UltraDuty = 100;

static xTaskHandle pInitTaskHandle = NULL;

static void HighProTask(void *pvParameters);
static void MediumProTask(void *pvParamters);
static void LowProTask(void *pvParameters);
/*!
 *@brief:	 APP_Start
 *@param:	 none
 *@retval: none
 *@none:	 功能处理函数，根据模块需求进行分类
 */
 void APP_Start(void)
{
	xTaskCreate(InitTask1,
				(portCHAR const *)"vInitTask",
				128,
				NULL,
				5,
				&pInitTaskHandle);
}


void InitTask1(void *pvParameters)
{

	xTaskCreate(HighProTask,
				(portCHAR const *)"HighProTask",
				HIGHPROTASK_STACK_SIZE,
				NULL,
				TASK_PRIO_HIGH,
				NULL);

	xTaskCreate(MediumProTask,
				(portCHAR const *)"MediumProTask",
				HIGHPROTASK_STACK_SIZE,
				NULL,
				TASK_PRIO_HIGH,
				NULL);

	xTaskCreate(LowProTask,
				(portCHAR const *)"LowProTask",
				LOWPROTASK_STACK_SIZE,
				NULL,
				TASK_PRIO_LOW,
				NULL);

	vTaskDelete(pInitTaskHandle);
}

static void HighProTask(void *pvParamters)
{
	StandyTime = 0;
	UltraParam_Init();
	for (;;)
	{
		Lipus_MainFunc();
		Power_MainFunc();
	}
}

static void MediumProTask(void *pvParamters)
{
	for (;;)
	{
		//IWDG_ReloadCounter();
		ADC_MainFunc();
		DevBeep_MainFunc();
		vTaskDelay(5);
	}
}

static void LowProTask(void *pvParamters)
{
	LED_Init();
	for (;;)
	{
		DevLed_MainFunc();
		vTaskDelay(1000);
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
	uint8_t dev_work_sta = IDLE_STATE;
	uint8_t duty = 0;

	duty = UltraDuty;
	if(Battery_vol < 3300)
	{
			DevWorkState = CLOSE_STATE;
	}
	dev_work_sta = DevWorkState;
	
	switch (dev_work_sta)
	{
	case IDLE_STATE:

		if(StandyTime > 9000)
		{
			DevWorkState = CLOSE_STATE;
			PowerOff_Flg = 1;
		}
		// 设置相关GPIO输出为低电平
		DevGpio_SetOutPut(V12_EN, Bit_RESET);
		DevGpio_SetOutPut(V45_EN, Bit_RESET);
		DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
		Devpwm_SetDuty(SET_PWM1, 0);
		break;
	case PAUSE_STATE:

		StandyTime = 0;

		DevGpio_SetOutPut(V12_EN, Bit_RESET);
		DevGpio_SetOutPut(V45_EN, Bit_RESET);
		DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
		Devpwm_SetDuty(SET_PWM1, 0);
		break;

	case CHARGE_STATE:

		StandyTime = 0;

		DevGpio_SetOutPut(V12_EN, Bit_RESET);
		DevGpio_SetOutPut(V45_EN, Bit_RESET);
		DevGpio_SetOutPut(WAVE_EN, Bit_RESET);
		Devpwm_SetDuty(SET_PWM1, 0);
		POWER_OFF();
		break;

	case WORK_STATE:

		StandyTime = 0;
		
		DevGpio_SetOutPut(V12_EN, Bit_SET);
		DevGpio_SetOutPut(V45_EN, Bit_SET);
		DevGpio_SetOutPut(WAVE_EN, Bit_SET);
		//DevGpio_SetOutPut(MOTOR_GATE, Bit_SET);
		Devpwm_SetDuty(SET_PWM1, duty);
		break;

	case CLOSE_STATE:

		Power_MainFunc();
		break;

	default:
		break;
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
		POWER_OFF();
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
			POWER_OFF();
	}
}
