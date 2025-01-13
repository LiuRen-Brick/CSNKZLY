/**
  *********************************************************************************
  * @file    	    main.c
  * @author  	    FMD AE
  * @brief   		main program body 	
  * @version 	    V1.0.0           
  * @data		    2021-09-27
  *********************************************************************************
  * @attention
  * COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
  *    This software is provided by the copyright holders and contributors,and the
  *software is believed to be accurate and reliable. However, Fremont Micro Devices
  *(SZ) Corporation assumes no responsibility for the consequences of use of such
  *software or for any infringement of patents of other rights of third parties,
  *which may result from its use. No license is granted by implication or otherwise
  *under any patent rights of Fremont Micro Devices (SZ) Corporation.
  *  ******************************************************************************
  */
	
	/*
 * Keil 编译后 Flash 和 RAM 占用情况：
 * Program Size: Code=15236 RO-data=1708 RW-data=240 ZI-data=5440
 *
 * Code -- 代码
 * RO-data -- 常量
 * RW-data -- 可读写变量，已初始化
 * ZI-data -- 可读写变量，未初始化
 *
 * 以上flash和ram占用情况：flash = 64K	RAM = 8K
 * flash = Code + RO-data + RW-data = 15236 + 1708 + 240 = 17184 bytes = 16.78kb
 * ram = RW-data + ZI-data = 240 + 5440 = 5680 bytes = 5.55kb
 */
/* Includes ----------------------------------------------------------------------*/
#include "main.h"

#include "dev_adc.h"
#include "dev_ad9877.h"
#include "dev_gpio.h"
#include "dev_pwm.h"
#include "dev_flash.h"
#include "dev_beep.h"
#include "App.h"

static void FT32_GPIOA_Init(void);
static void FT32_GPIOB_Init(void);
static void FT32_EXIT_Init(void);
static void ADC_DMA_Config(void);
static void Tim3BaseInit(void);
static void Tim15BaseInit(void);
static void Tim16BaseInit(void);
static void Tim17BaseInit(void);
static void Tim14BaseInit(void);
static void IWDG_Config(void);

uint8_t PowerOff_Flg = 1;
extern BEEP_STATE BeepState;

uint32_t DMA_DualConvertedValue[1] = {0};
__IO uint32_t lsi_freq = 40000;

/**********************************************************************************
  * @brief  main program
  * @param  
  * @note  
  * @retval 
  *********************************************************************************
*/
int main(void)
{      
	FT32_GPIOA_Init();
	FT32_GPIOB_Init();
	FT32_EXIT_Init();
	ADC_DMA_Config();
	/*超声脉冲驱动占空比*/
	Tim3BaseInit();
	/*蜂鸣器*/
	Tim15BaseInit();
	/*马达驱动*/
	Tim16BaseInit();
	/*定时器*/
	Tim17BaseInit();
	Tim14BaseInit();
	
	//SysTick_Configuration();
	/*长按开机*/
	while(PowerOff_Flg);
	POWER_ON();

 	/*看门狗*/
	//IWDG_Config();
	APP_Start();
	vTaskStartScheduler();            // System scheduling
	while(1)
	{
			
	}
	return (0);       
}

/************************ (C) COPYRIGHT Fremont Micro Devices *****BEGIN OF FILE****/
void FT32_GPIOA_Init(void)
{
	  GPIO_InitTypeDef	GPIO_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
		//PA7-Motor_GATE  PA8-12V_EN PA9-GPIO_OUT 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//PA10-GPIO_IN
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
		//PA15-GPIO_IN
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
		GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void FT32_GPIOB_Init(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
		GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);
		GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void FT32_EXIT_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);

  EXTI_InitStructure.EXTI_Line = EXTI_Line4 | EXTI_Line5;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
}

void ADC_DMA_Config(void)
{
		ADC_InitTypeDef	ADC_InitStructure;
		GPIO_InitTypeDef	GPIO_InitStructure;
		DMA_InitTypeDef	DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStruct;
	
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC Channel11 and channel10 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 

  /* Convert the ADC1 Channel0 and channel1 with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_55_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles); 
   
  /* ADC 标定 */
  ADC_GetCalibrationFactor(ADC1);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* 使能 ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE); 
	
  /* 使能ADC外设 */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* 等待ADC1准备就绪 */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
	
	/* 配置DMA */
  /* 开启DMA1 时钟*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* 配置DMA1 Channel1 */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_DualConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* 使能DMA1 的通道1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/*
	//ADC 中断处理函数配置
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	*/
	/* ADC1 regular Software Start Conv */ 
	//开启ADC数据转换//
  ADC_StartOfConversion(ADC1);
  /* Infinite loop */
}



//TIM3_CH1  PB4
//超声脉冲周期设置
void Tim3BaseInit(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_BaseConfigStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);

	TIM_BaseConfigStruct.TIM_RepetitionCounter = 0;
	TIM_BaseConfigStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
	TIM_BaseConfigStruct.TIM_CounterMode  = TIM_CounterMode_Up;
	TIM_BaseConfigStruct.TIM_Prescaler  = 72-1;
	TIM_BaseConfigStruct.TIM_Period = 10000-1;
	
	TIM_TimeBaseInit(TIM3,&TIM_BaseConfigStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	/* TIM3 counter enable */
	TIM_Cmd(TIM3, ENABLE);

	/* TIM3 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM3, ENABLE);    
}

//TIM15 Ch1，PA2
//蜂鸣器驱动  4K
void Tim15BaseInit(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_BaseConfigStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_0);

	TIM_BaseConfigStruct.TIM_RepetitionCounter = 0;
	TIM_BaseConfigStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
	TIM_BaseConfigStruct.TIM_CounterMode  = TIM_CounterMode_Up;
	TIM_BaseConfigStruct.TIM_Prescaler  = 72-1;
	TIM_BaseConfigStruct.TIM_Period = 250-1;
	
	TIM_TimeBaseInit(TIM15,&TIM_BaseConfigStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM15,&TIM_OCInitStruct);
	/* TIM15 counter enable */
	TIM_Cmd(TIM15, ENABLE);

	/* TIM15 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM15, ENABLE);    
}
//TIM16_CH1
//PA6  电机驱动
//235Hz为周期
void Tim16BaseInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_BaseConfigStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);

	TIM_BaseConfigStruct.TIM_RepetitionCounter = 0;
	TIM_BaseConfigStruct.TIM_ClockDivision  = TIM_CKD_DIV1;
	TIM_BaseConfigStruct.TIM_CounterMode  = TIM_CounterMode_Up;
	TIM_BaseConfigStruct.TIM_Prescaler  = 72-1;
	TIM_BaseConfigStruct.TIM_Period = 4254-1;
	
	TIM_TimeBaseInit(TIM16,&TIM_BaseConfigStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM16,&TIM_OCInitStruct);
	/* TIM16 counter enable */
	TIM_Cmd(TIM16, ENABLE);

	/* TIM16 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM16, ENABLE); 
}


/*
 *    定时器17 作为时间累加  1ms为周期
 */
void Tim17BaseInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE );

	/* Enable the TIM17 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* TIM17 counter enable */
	TIM_Cmd(TIM17, ENABLE);
}

/*
 *    定时器14 作为时间累加  5ms为周期
 */
#if 1
void Tim14BaseInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
	/* TIM1 clock enable */

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 5030-1;						//实际测试中，定时器比实际时间快做30us补偿
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	
	/* Enable the TIM17 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM17 counter enable */
	TIM_Cmd(TIM14, ENABLE);
}
#endif
/**
  * @brief  独立看门狗配置程序.
  * @param  None
  * @retval None
  */
void IWDG_Config(void)
{
  /* 使能独立看门狗 */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG 时钟预分频: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* 设置看门狗计数重载值，超时时间为250ms.
     Counter Reload Value = 250ms/IWDG counter clock period
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(lsi_freq/128);

  /* 计数重载(喂狗) */
  IWDG_ReloadCounter();

  /* 使能看门狗功能 */
  IWDG_Enable();	
}
/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/


