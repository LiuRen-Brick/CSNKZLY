/**
  *********************************************************************************
  * @file    	    TK_LIB.h
  * @author  	    FMD AE
  * @brief   		TK_LIB Header File. 	
  * @version 	    V1.0.0           
  * @data		    2021-09-15
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

#ifndef     __TK_LIB_H
#define     __TK_LIB_H

/* Includes ---------------------------------------------------------------------*/
#include  "ft32f0xx.h"
#include  "ft32f072x8.h"

/* Public Constant --------------------------------------------------------------*/

/* Public typedef ---------------------------------------------------------------*/
typedef struct
{
	uint8_t    MeantimeMaxKeyNum;					// Maximum number of keys that can be pressed simultaneously
	uint8_t    Multiple;							// Magnification of the scan data of the key
    uint8_t    SingleFilter;						// Number of the key filters
    uint8_t    StrongFilter;						// Number of strongest key filters
	uint8_t    BaselineUpSpeed;						// baseline upward update speed
	uint8_t    BaselineDownSpeed;					// Baseline downward update speed
	uint8_t    BaselineEqualSpeed;				    // Baseline rapid update speed
	uint8_t    SignalNoiseRatio;					// Input SNR
}TSC_ParaDef;

/* Public define ----------------------------------------------------------------*/

/* Public variables -------------------------------------------------------------*/
extern uint8_t ucFrequNum;

/* Public function prototypes----------------------------------------------------*/
void TSC_EnableKey1(uint8_t KeySensitivity);
void TSC_EnableKey2(uint8_t KeySensitivity);
void TSC_EnableKey3(uint8_t KeySensitivity);
void TSC_EnableKey4(uint8_t KeySensitivity);
void TSC_EnableKey5(uint8_t KeySensitivity);
void TSC_EnableKey6(uint8_t KeySensitivity);
void TSC_EnableKey7(uint8_t KeySensitivity);
void TSC_EnableKey8(uint8_t KeySensitivity);
void TSC_EnableKey9(uint8_t KeySensitivity);
void TSC_EnableKey10(uint8_t KeySensitivity);
void TSC_EnableKey11(uint8_t KeySensitivity);
void TSC_EnableKey12(uint8_t KeySensitivity);
void TSC_EnableKey13(uint8_t KeySensitivity);
void TSC_EnableKey14(uint8_t KeySensitivity);
void TSC_EnableKey15(uint8_t KeySensitivity);
void TSC_EnableKey16(uint8_t KeySensitivity);
void TSC_EnableKey17(uint8_t KeySensitivity);
void TSC_EnableKey18(uint8_t KeySensitivity);
void TSC_EnableKey19(uint8_t KeySensitivity);
void TSC_EnableKey20(uint8_t KeySensitivity);
void TSC_EnableKey21(uint8_t KeySensitivity);
void TSC_EnableKey22(uint8_t KeySensitivity);
void TSC_EnableKey23(uint8_t KeySensitivity);
void TSC_EnableKey24(uint8_t KeySensitivity);
void TSC_Start(void);
void TSC_DataProcessing(void);
uint8_t  TSC_GetStrongest(void);
uint32_t TSC_GetSingle(void);
void TSC_Reset(void);
uint8_t TSC_GetDelta(uint8_t KeyNum);
void Set_Baseline(uint8_t KeyNum,uint16_t BaselineData);
uint16_t TSC_GetPrevData(uint8_t KeyNum);
void TSC_Init(TSC_ParaDef *TSC_ParaStruct);
void Set_KeyFilter(uint8_t SingleFilter,uint8_t StrongFilter);
void Set_BaselineSpeed(uint8_t BaselineUpSpeed,uint8_t BaselineDownSpeed,uint8_t BaselineEqualSpeed);
void Set_SignalNoiseRatio(uint8_t iSignalNoiseRatio);
void Set_Frequ(uint8_t Frequ1,uint8_t Frequ2,uint8_t Frequ3);
uint8_t Get_Frequ(uint8_t FrequNum);
#endif

/************************* (C) COPYRIGHT FMD *****END OF FILE**********************/
