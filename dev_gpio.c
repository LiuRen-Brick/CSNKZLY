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
};

void DevGpio_SetOutPut(uint8_t ch,BitAction BitVal)
{
	GPIO_TypeDef* gpio;
	uint16_t pin;
	
	gpio = dev_gpio_handle[ch].GPIOx;
	pin = dev_gpio_handle[ch].GPIO_Pin;
	
	GPIO_WriteBit(gpio, pin, BitVal);
}





