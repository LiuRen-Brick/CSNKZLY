#include "dev_pwm.h"

#define PWMPRECENT	4

typedef void (* SetPWMDuty)(uint16_t duty);
static void Set_PWM1_Duty(uint16_t duty);
static void Set_PWM2_Duty(uint16_t duty);
static void Set_PWM3_Duty(uint16_t duty);

static const SetPWMDuty set_pwm_duty[SET_PWM_MAX_NUM] = 
{
	[SET_PWM1] = Set_PWM1_Duty,
	[SET_PWM2] = Set_PWM2_Duty,
	[SET_PWM3] = Set_PWM3_Duty,
};

//设置Tim3-pwm1占空比
//超声脉冲
static void Set_PWM1_Duty(uint16_t duty)
{
	uint32_t compare = 0;
	compare = duty * 100;
	TIM_SetCompare1(TIM3,compare);
}

//设置Tim14-pwm1占空比
static void Set_PWM2_Duty(uint16_t duty)
{
	uint32_t compare = 0;
	compare = duty * 2.5;
	TIM_SetCompare1(TIM15,compare);
}

//设置Tim16-pwm1占空比
//电机驱动
static void Set_PWM3_Duty(uint16_t duty)
{
	uint32_t compare = 0;
	compare = duty * 42.54;
	TIM_SetCompare1(TIM16,compare);
}

/*!
 *@brief	设置PWM输出占空比函数
 *@param	channle	需要设置的PWM的通道号
 *@param	duty		设置PWM输出占空比
 *@retval	void
 */
void Devpwm_SetDuty(uint8_t channle,uint16_t duty)
{
	set_pwm_duty[channle](duty);
}


