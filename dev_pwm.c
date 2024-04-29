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

//����Tim3-pwm1ռ�ձ�
static void Set_PWM1_Duty(uint16_t duty)
{
		uint32_t compare = 0;
		compare = duty * PWMPRECENT;
		TIM_SetCompare1(TIM3,compare);
}

//����Tim3-pwm4ռ�ձ�
static void Set_PWM2_Duty(uint16_t duty)
{
		uint32_t compare = 0;
		compare = duty * PWMPRECENT;
		TIM_SetCompare4(TIM3,compare);
}

//����Tim16-pwm1ռ�ձ�
static void Set_PWM3_Duty(uint16_t duty)
{
		uint32_t compare = 0;
		compare = duty * PWMPRECENT;
		TIM_SetCompare1(TIM16,compare);
}

/*!
 *@brief	��ʩPEMռ�ձȺ���
 *@param	channle	��Ҫ���õ�PWM��ͨ����
 *@param	duty		����PWM���ռ�ձ�
 *@retval	void
 */
void Devpwm_SetDuty(uint8_t channle,uint16_t duty)
{
		set_pwm_duty[SET_PWM1](duty);
}


