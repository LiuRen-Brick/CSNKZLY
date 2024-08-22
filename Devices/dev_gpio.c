#include "dev_gpio.h"

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}DevGpioHandleType;

#define GPIO01	GPIOB,GPIO_Pin_8
#define GPIO02	GPIOB,GPIO_Pin_9
#define GPIO03	GPIOA,GPIO_Pin_7
#define GPIO04	GPIOB,GPIO_Pin_1
#define GPIO05	GPIOA,GPIO_Pin_8
#define GPIO06	GPIOB,GPIO_Pin_3
#define GPIO07	GPIOB,GPIO_Pin_7
#define GPIO08	GPIOB,GPIO_Pin_6
#define GPIO09	GPIOB,GPIO_Pin_5
#define GPIO10	GPIOA,GPIO_Pin_9
#define GPIO11	GPIOA,GPIO_Pin_10
#define GPIO12	GPIOB,GPIO_Pin_0

uint8_t PowerFlg = 0;

static DevGpioHandleType dev_gpio_handle[DevGpioMaxChannlLab] = 
{
	[LED_RED]    = {GPIO01},
	[LED_GREEN]  = {GPIO02},
	[MOTOR_GATE] = {GPIO03},
	[V45_EN]     = {GPIO04},
	[V12_EN]     = {GPIO05},
	[WAVE_EN]    = {GPIO06},
	[A_SCLK]     = {GPIO08},
	[A_SDATA]    = {GPIO07},
	[A_FSYNC]    = {GPIO09},
	[POWER]		   = {GPIO10},
	[SWITCH]	   = {GPIO11},
	[MONIT]			 = {GPIO12},
};  

/*!
*@brief			����ָ��GPIOͨ�������ֵ
*@param			ch ͨ����
*@param			BitVal 0:�͵�ƽ 1:�ߵ�ƽ
*@retval		none
*@note			no
*/
void DevGpio_SetOutPut(uint8_t ch, BitAction BitVal)
{
	GPIO_TypeDef* gpio;
	uint16_t pin;

	// ��ȡָ��ͨ����GPIO�˿�
	gpio = dev_gpio_handle[ch].GPIOx;
	// ��ȡָ��ͨ����GPIO����
	pin = dev_gpio_handle[ch].GPIO_Pin;

	// ����GPIO���ŵ����ֵ
	GPIO_WriteBit(gpio, pin, BitVal);
}

/*!
*@brief			��ȡIO�ڵĵ�ƽ�ź�
*@param			ch ͨ����
*@retval		gpio_sta IO�ڵ�ƽ״̬
*@note			no
*/
uint8_t DevGpio_ReadInPut(uint8_t ch)
{
	uint16_t gpio_sta;
	GPIO_TypeDef* gpio;
	uint16_t pin;
	
	gpio = dev_gpio_handle[ch].GPIOx;
	pin = dev_gpio_handle[ch].GPIO_Pin;
	
	gpio_sta = GPIO_ReadInputDataBit(gpio, pin);
	
	return gpio_sta;
}

/**
 * @brief �л�ָ��GPIO���ŵĵ�ƽ״̬
 *
 * �ú��������л�ָ��GPIO���ŵĵ�ƽ״̬��������ŵ�ǰΪ�ߵ�ƽ��������Ϊ�͵�ƽ��
 ������ŵ�ǰΪ�͵�ƽ��������Ϊ�ߵ�ƽ��
 *
 * @param ch Ҫ�л���ƽ״̬��GPIO���ű��
 */
void GPIO_ToggleBit(uint8_t ch)
{
	/* ������ */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	assert_param(IS_GPIO_BIT_ACTION(BitVal));
 
	GPIO_TypeDef* gpio;
	uint16_t pin;
 
	/* ��ȡ GPIO �˿ں����� */
	gpio = dev_gpio_handle[ch].GPIOx;
	pin = dev_gpio_handle[ch].GPIO_Pin;

	/* ���ݵ�ǰ��ƽ״̬�л����ŵ�ƽ */
	if((gpio->ODR & pin) != (uint32_t)Bit_RESET)
	{
	gpio->BRR = pin ;
	}else
	{
	gpio->BSRR = pin;
	}
}

