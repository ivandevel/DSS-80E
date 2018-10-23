#include "stm8s.h"
#include "stm8s_eval.h"

#include "thermo.h"

void Temperature_ADC_Config (void)
{
  /*  Init GPIO for ADC2 */
  GPIO_Init(ADC_GPIO_PORT, ADC_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
  
  // Deinit ADC
  ADC1_DeInit();
  
  ADC1_Init(
    ADC1_CONVERSIONMODE_CONTINUOUS,
    ADC1_CHANNEL_4,
    ADC1_PRESSEL_FCPU_D4,
    ADC1_EXTTRIG_TIM, DISABLE,
    ADC1_ALIGN_RIGHT,
    ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);

  ADC1_Cmd(ENABLE);
}

uint16_t GetAdcValue(ADC1_Channel_TypeDef channel)
{
  ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, channel, ADC1_ALIGN_RIGHT);
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  return ADC1_GetConversionValue();
}

//#define ARR_SIZE 
//#pragma optimize=none
static int16_t Convert(int16_t adc_code, int16_t tcj) 
{
uint8_t x;
uint16_t Vread;

#ifdef DFS_90
Vread = Code2uV(adc_code);
#endif

#ifdef SOLDERING_HAKKO_A1321
Vread = Code2Resistance(adc_code);
#endif

#ifdef SOLDERING_HAKKO_A1321_MINI
Vread = Code2Resistance(adc_code);
#endif

for (x = 0; x < NELEMS(vArray) - 2; x+=2)
{
 if (Vread >= vArray[x] && Vread <= vArray[x+2]) 
 {
   return vArray[x+1] + (int16_t)((int32_t)(vArray[x+3] - vArray[x+1]) * (int32_t)(Vread - vArray[x]) / (int32_t)(vArray[x+2] - vArray[x]));  
 }
}
return 999;
}

//Calculate microvolts from OPAMP
//#pragma optimize=none
static int32_t Code2uV(int16_t adccode)
{
  //5350 - коэффициент усилителя 535.0
  //1023 - разрешающая способность АЦП
  //510000000 - опорное напряжение АЦП 5 Вольт
  return (((uint32_t)adccode)*((5100000UL/1023UL)/OPAMP_GAIN));///1000;
}


//Calculate input voltage from OPAMP
//#pragma optimize=none
static int32_t Code2Resistance(int16_t adccode)
{
  float mvolt;
  
  mvolt = (((uint32_t)adccode)*((5.1/1023)/OPAMP_GAIN));
// 1) Uo = (5.0 / 1023.0) * adccode;
// 2) Ui = (Uo-(Vcc*Rfb/R1)) / (1 + Rfb/R2 + Rfb/R1)  
// 3) R2 = 1000 / ((5.0 / Ui) - 1); 
   
  return (int16_t)(1000UL / ((5.1 / mvolt) - 1));
}

static int32_t varVolt = 112;
static int32_t varProcess = 100;
static int32_t Pc = 0;
static int32_t G = 0;
static int32_t P=100000;
static int32_t Xp = 0.0;
static int32_t Zp = 0;
static int32_t Xe=0;

static int32_t Kalman(int32_t voltage)
{
Pc = P+varProcess;
G = Pc/(Pc+varVolt);
P = (100000-G)*Pc;
Xp = Xe;
Zp = Xp;
Xe = G * (voltage - Zp) + Xp;

return Xe;
}

int16_t Get_Temperature_Filtered(uint32_t adc_raw)
{
return Kalman(Convert(adc_raw, 1));
}