#include "dev_pwm.h"

#define PWMPRECENT	4

typedef void (* SetPWMDuty)(uint16_t duty);
static void Set_PWM1_Duty(uint16_t duty);
static void Set_PWM14_Duty(uint16_t duty);

static const SetPWMDuty set_pwm_duty[SET_PWM_MAX_NUM] = 
{
	[SET_PWM1] = Set_PWM1_Duty,
	[SET_PWM14] = Set_PWM14_Duty,
};

static void Set_PWM1_Duty(uint16_t duty)
{
		uint32_t compare = 0;
		compare = duty * PWMPRECENT;
		TIM_SetCompare1(TIM3,compare);
}

static void Set_PWM14_Duty(uint16_t duty)
{
		uint32_t compare = 0;
		compare = duty * PWMPRECENT;
		TIM_SetCompare1(TIM14,compare);
}

void Devpwm_SetDuty(uint8_t channle,uint16_t duty)
{
		set_pwm_duty[SET_PWM1](duty);
}

