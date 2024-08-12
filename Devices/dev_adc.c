#include "main.h"
#include "dev_adc.h"

static float ADCSample_Filter(float buff[],uint8_t len);

float SampleTemp_Buff[16];
float SampleVol_Buff[16] = {3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500,3500};

extern uint32_t DMA_DualConvertedValue[2];
float Battery_vol = 0;
float Ultra_Temp = 0;

//PT100  电阻表
const float INP_HWTemp1_R[160] = 
{
	88.22f,	 88.62f,	89.01f,	 89.40f,	89.80f,  90.19f,  90.59f,  90.98f,  91.37f,  91.77f, //-30℃~-21℃
	92.16f,  92.55f,  92.95f,  93.34f,  93.73f,  94.12f,  94.52f,  94.91f,  95.30f,	 95.69f, //-20℃~-11℃
  96.09f,  96.48f,  96.87f,  97.26f,  97.65f,  98.04f,  98.44f,  98.83f,  99.22f,  99.61f, //-10℃~-1℃
	100.00f, 100.39f, 100.78f, 101.17f, 101.56f, 101.95f, 102.34f, 102.73f, 103.12f, 103.51f,//0℃~9℃
	103.90f, 104.29f, 104.68f, 105.07f, 105.46f, 105.85f, 106.24f, 106.63f, 107.02f, 107.40f,//10℃~19℃
	107.79f, 108.18f, 108.57f, 108.96f, 109.35f, 109.73f, 110.12f, 110.51f, 110.90f, 111.28f,//20℃~29℃
	111.67f, 112.06f, 112.45f, 112.83f, 113.22f, 113.61f, 113.99f, 114.38f, 114.77f, 115.15f,//30℃~39℃
	115.54f, 115.93f,	116.31f, 116.70f, 117.08f, 117.47f, 117.85f, 118.24f, 118.62f, 119.01f,//40℃~49℃
	119.40f, 119.78f, 120.16f, 120.55f, 120.93f, 121.32f, 121.70f, 122.09f, 122.47f, 122.86f,//50℃~59℃
	123.24f, 123.62f, 124.01f, 124.39f, 124.77f, 125.17f, 125.55f, 125.93f, 126.32f, 126.70f,//60℃~69℃
	127.08f, 127.46f,	127.85f, 128.23f, 128.61f, 128.99f, 129.38f, 129.76f, 130.14f, 130.52f,//70℃~79℃
	130.90f, 131.28f,	131.67f, 132.05f,	132.43f, 132.81f, 133.19f, 133.57f, 133.95f, 134.33f,//80℃~89℃
	134.71f, 135.09f,	135.47f, 135.85f, 136.23f, 136.61f, 136.99f, 137.37f, 137.75f, 138.13f,//90℃~99℃
	138.51f, 138.89f,	139.27f, 139.65f, 140.03f, 140.39f,	140.77f, 141.15f, 141.53f, 141.91f,//100℃~109℃
	142.29f, 142.66f,	143.04f, 143.42f, 143.80f, 144.18f, 144.56f, 144.94f, 145.31f, 145.69f,//110℃~119℃
	146.07f, 146.44f, 146.82f, 147.20f, 147.57f, 147.95f, 148.33f, 148.70f, 149.08f, 149.46f,//120℃~129℃
};

//PT100 温度表
const float OUT_HWTemp1_T[160] = 
{	
	-30.0f,	-29.0f,	-28.0f,	-27.0f,	-26.0f,	-25.0f,	-24.0f,	-23.0f,	-22.0f,	-21.0f,
	-20.0f,	-19.0f,	-18.0f,	-17.0f,	-16.0f,	-15.0f,	-14.0f, -13.0f,	-12.0f,	-11.0f,
	-10.0f,	 -9.0f,	 -8.0f,  -7.0f,  -6.0f,  -5.0f,  -4.0f,  -3.0f,	 -2.0f,  -1.0f, 
	  0.0f,   1.0f,   2.0f,   3.0f,  	4.0f,  	5.0f,  	6.0f,  	7.0f,  	8.0f,  	9.0f,  
	 10.0f,  11.0f,  12.0f,  13.0f,  14.0f,  15.0f,  16.0f,  17.0f,  18.0f,  19.0f,  
	 20.0f,  21.0f,  22.0f,  23.0f,  24.0f,  25.0f,  26.0f,  27.0f,  28.0f,  29.0f, 
	 30.0f,  31.0f,  32.0f,  33.0f,  34.0f,  35.0f,  36.0f,  37.0f,  38.0f,  39.0f, 
	 40.0f,  41.0f,  42.0f,  43.0f,  44.0f,  45.0f,  46.0f,  47.0f,  48.0f,  49.0f,
	 50.0f,  51.0f,  52.0f,  53.0f,  54.0f,  55.0f,  56.0f,  57.0f,  58.0f,  59.0f, 
	 60.0f,  61.0f,  62.0f,  63.0f,  64.0f,  65.0f,  66.0f,  67.0f,  68.0f,  69.0f,
	 70.0f,  71.0f,  72.0f,  73.0f,  74.0f,  75.0f,  76.0f,  77.0f,  78.0f,  79.0f, 
	 80.0f,  81.0f,  82.0f,  83.0f,	 84.0f,  85.0f,  86.0f,  87.0f,	 88.0f,  89.0f, 
	 90.0f,  91.0f,  92.0f,  93.0f,  94.0f,  95.0f,  96.0f,  97.0f,	 98.0f,  99.0f, 
	 100.0f,101.0f,	102.0f,	103.0f,	104.0f,	105.0f, 106.0f, 107.0f, 108.0f,	109.0f,
	 110.0f,111.0f,	112.0f, 113.0f,	114.0f,	115.0f,	116.0f,	117.0f, 118.0f, 119.0f, 
	 120.0f,121.0f,	122.0f, 123.0f,	124.0f,	125.0f,	126.0f,	127.0f, 128.0f, 129.0f,
};

/**
 * @brief 使用二分查找法在查找表中进行插值查找
 *
 * 该函数使用二分查找法在查找表中查找给定的输入值 `u0`，并进行插值以获得更精确的结果。
 *
 * @param u0 输入值，用于在查找表中进行查找。
 * @param bp0 查找表的基点数组，包含查找表的各个基点值。
 * @param table 查找表数组，包含与基点对应的值。
 * @param maxIndex 查找表的最大索引值，表示查找表的大小减一。
 * @return 返回通过插值计算得到的查找结果。
 */
float look1_iflf_binlxpw(float u0, const float bp0[], const float table[], uint32_t maxIndex)
{
  float frac;
  uint32_t iRght;
  uint32_t iLeft;
  uint32_t bpIdx;

  if (u0 <= bp0[0U])
  {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  }
  else if (u0 < bp0[maxIndex])
  {
    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U)
    {
      if (u0 < bp0[bpIdx])
      {
        iRght = bpIdx;
      }
      else
      {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  }
  else
  {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  return (table[iLeft + 1U] - table[iLeft]) * frac + table[iLeft];
}

/*!
*@brief			电压转换函数
*@param			void
*@retval		void
*@note			ADC原始数据转化为电压值
*/
void ADC_MainFunc(void)
{
	uint16_t TempDualVal = 0;
	uint16_t VolDualVal = 0;
	float Bat_vol = 0;
	static uint8_t SampleCount = 0;
	static float Vol = 0.0f;
	static float Temp_V = 0.0f;
	static float Temp_R = 0.0f;
	static float Temp_T = 0.0f;
	//判断DMA传输是否完成，准备下一次ADC采样
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET)
	{
		TempDualVal = (DMA_DualConvertedValue[0] & 0xFFFF);
		VolDualVal = ((DMA_DualConvertedValue[0] >> 16) & 0xFFFF);
		//电池电压计算公式
		Bat_vol = VolDualVal * 3300 / 4096;
		SampleVol_Buff[SampleCount] = Bat_vol * 2.9437;
		//温度电压计算公式
		Vol = TempDualVal * 3.3 /4096;
		//Vout =  Gain * Vin;    (Gain = 1 + (100K/Rg))
		Temp_V = Vol / 11.0f;
		/* 电压 --- 电阻计算公式 (AD623)
			Vin = 3V*(Rpt / (510Ω + Rpt)) - 3V*(90Ω / (510Ω + 90Ω))
		*/
			
		Temp_R = (Temp_V * 510 + 229.5) / (3.0f  - 0.45  - Temp_V);
		//查表，根据电阻值查询对应温度
		Temp_T = look1_iflf_binlxpw(Temp_R, INP_HWTemp1_R, OUT_HWTemp1_T, 159);
		SampleTemp_Buff[SampleCount] = Temp_T;
		Ultra_Temp = ADCSample_Filter(SampleTemp_Buff,15) - 1.5;
		Battery_vol = ADCSample_Filter(SampleVol_Buff,15);
		SampleCount++;
		if(SampleCount > 15)
		{
				SampleCount = 0;
		}
		DMA_ClearFlag(DMA1_FLAG_TC1);
	}
}

static float ADCSample_Filter(float buff[],uint8_t len)
{
		if (len < 3) {
        // 如果数组元素少于3个，无法去掉最大最小值后计算平均值
        return 0.0;
    }

    float max = buff[0];
    float min = buff[0];
    float sum = 0;

    // 找到最大值、最小值并计算总和
    for (int i = 0; i < len; i++) {
        if (buff[i] > max) {
            max = buff[i];
        }
        if (buff[i] < min) {
            min = buff[i];
        }
        sum += buff[i];
    }

    // 从总和中减去最大值和最小值
    sum -= max;
    sum -= min;

    // 计算剩余元素的平均值
    double average = (double)sum / (len - 2);
    return average;
}
