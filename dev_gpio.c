#include "dev_gpio.h"

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}DevGpioHandleType;

#define GPIO01	GPIOA,GPIO_Pin_2
#define GPIO02	GPIOA,GPIO_Pin_3
#define GPIO03	GPIOA,GPIO_Pin_6
#define GPIO04	GPIOA,GPIO_Pin_9
#define GPIO05	GPIOA,GPIO_Pin_10
#define GPIO06	GPIOB,GPIO_Pin_3
#define GPIO07	GPIOB,GPIO_Pin_5
#define GPIO08	GPIOB,GPIO_Pin_6
#define GPIO09	GPIOB,GPIO_Pin_7
#define GPIO10	GPIOB,GPIO_Pin_8
#define GPIO11	GPIOB,GPIO_Pin_9

static DevGpioHandleType dev_gpio_handle[DevGpioMaxChannlLab] = 
{
	[LED_RED]    = {GPIO01},
	[LED_GREEN]  = {GPIO02},
	[MOTOR_GATE] = {GPIO03},
	[V45_EN]     = {GPIO04},
	[V12_EN]     = {GPIO05},
	[WAVE_EN]    = {GPIO06},
	[A_SCLK]     = {GPIO07},
	[A_SDATA]    = {GPIO08},
	[A_FSYNC]    = {GPIO09},
	[BAT_STDBY]  = {GPIO10},
	[BAT_CHARGE]	 = {GPIO11},
};


/*!
*@brief			设置IO口电平信号
*@param			ch 通道号
*@param			BitVal 0:低电平 1:高电平
*@retval		gpio_sta IO口电平状态
*@note			no
*/
void DevGpio_SetOutPut(uint8_t ch,BitAction BitVal)
{
	GPIO_TypeDef* gpio;
	uint16_t pin;
	
	gpio = dev_gpio_handle[ch].GPIOx;
	pin = dev_gpio_handle[ch].GPIO_Pin;
	
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





