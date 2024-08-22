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
*@brief			设置指定GPIO通道的输出值
*@param			ch 通道号
*@param			BitVal 0:低电平 1:高电平
*@retval		none
*@note			no
*/
void DevGpio_SetOutPut(uint8_t ch, BitAction BitVal)
{
	GPIO_TypeDef* gpio;
	uint16_t pin;

	// 获取指定通道的GPIO端口
	gpio = dev_gpio_handle[ch].GPIOx;
	// 获取指定通道的GPIO引脚
	pin = dev_gpio_handle[ch].GPIO_Pin;

	// 设置GPIO引脚的输出值
	GPIO_WriteBit(gpio, pin, BitVal);
}

/*!
*@brief			获取IO口的电平信号
*@param			ch 通道号
*@retval		gpio_sta IO口电平状态
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
 * @brief 切换指定GPIO引脚的电平状态
 *
 * 该函数用于切换指定GPIO引脚的电平状态。如果引脚当前为高电平，则将其置为低电平；
 如果引脚当前为低电平，则将其置为高电平。
 *
 * @param ch 要切换电平状态的GPIO引脚编号
 */
void GPIO_ToggleBit(uint8_t ch)
{
	/* 检查参数 */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
	assert_param(IS_GPIO_BIT_ACTION(BitVal));
 
	GPIO_TypeDef* gpio;
	uint16_t pin;
 
	/* 获取 GPIO 端口和引脚 */
	gpio = dev_gpio_handle[ch].GPIOx;
	pin = dev_gpio_handle[ch].GPIO_Pin;

	/* 根据当前电平状态切换引脚电平 */
	if((gpio->ODR & pin) != (uint32_t)Bit_RESET)
	{
	gpio->BRR = pin ;
	}else
	{
	gpio->BSRR = pin;
	}
}

