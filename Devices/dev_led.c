#include "App.h"
#include "main.h"
#include "App.h"
#include "dev_gpio.h"

uint16_t LCD_ToggleTime = 0;
extern DEV_WORK_STATE DevWorkState;
extern float Battery_vol;

static uint8_t BatLow_Flag = 0;

static void LED_Toggle(uint8_t led_colour);

void LED_Init(void)
{
    DevGpio_SetOutPut(LED_RED, Bit_SET);     // 设置红色LED灯为关闭状态
    DevGpio_SetOutPut(LED_GREEN, Bit_RESET); // 设置绿色LED灯为开启状态
}

void DevLed_MainFunc(void)
{
    uint8_t led_sta = 0;

    led_sta = DevWorkState;

    if (Battery_vol < 3500)
    {
        BatLow_Flag = 1;
    }
    
    switch (led_sta)
    {
    case IDLE_STATE:
        if(BatLow_Flag == 1)
        {
            DevGpio_SetOutPut(LED_GREEN, Bit_SET); // 设置绿色LED灯为开启状态
            LED_Toggle(LED_RED);
        }
        else
        {
            DevGpio_SetOutPut(LED_RED, Bit_SET); // 设置绿色LED灯为开启状态
            LED_Toggle(LED_GREEN);
        }
        break;
        
        break;
    case CHARGE_STATE:
        DevGpio_SetOutPut(LED_RED, Bit_SET);     
        DevGpio_SetOutPut(LED_GREEN, Bit_SET);  
        break;
    case PAUSE_STATE:
        if(BatLow_Flag == 1)
        {
            DevGpio_SetOutPut(LED_GREEN, Bit_SET);
            LED_Toggle(LED_RED);
        }
        else
        {
            DevGpio_SetOutPut(LED_RED, Bit_SET);
            LED_Toggle(LED_GREEN);
        }
        break;
    case WORK_STATE:
        if(BatLow_Flag == 1)
        {
            DevGpio_SetOutPut(LED_RED, Bit_RESET);   
            DevGpio_SetOutPut(LED_GREEN, Bit_SET); 
        }
        else
        {
            DevGpio_SetOutPut(LED_RED, Bit_SET);   
            DevGpio_SetOutPut(LED_GREEN, Bit_RESET); 
        }
        break;
    default:
        break;
    }
}

static void LED_Toggle(uint8_t led_colour)
{
    if(led_colour == LED_RED)
    {
        //if(LCD_ToggleTime > 100)
        {
            GPIO_ToggleBit(LED_RED);
            DevGpio_SetOutPut(LED_GREEN, Bit_SET); // 设置绿色LED灯为开启状态
            LCD_ToggleTime = 0;
        }
    }
    else if(led_colour == LED_GREEN)
    {
        //if(LCD_ToggleTime > 100)
        {
            GPIO_ToggleBit(LED_GREEN);
            DevGpio_SetOutPut(LED_RED, Bit_SET); // 设置绿色LED灯为开启状态
            LCD_ToggleTime = 0;
        }
    }
}