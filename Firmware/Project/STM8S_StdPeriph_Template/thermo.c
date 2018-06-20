#include "stm8s.h"
#include "thermo.h"

//#define ARR_SIZE 
#pragma optimize=none
int16_t Convert(int16_t adc_code, int16_t tcj) 
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
#pragma optimize=none
int32_t Code2uV(int16_t adccode)
{
  //5350 - коэффициент усилителя 535.0
  //1023 - разрешающая способность АЦП
  //510000000 - опорное напряжение АЦП 5 Вольт
  return (((uint32_t)adccode)*((5100000UL/1023UL)/OPAMP_GAIN));///1000;
}


//Calculate input voltage from OPAMP
#pragma optimize=none
int32_t Code2Resistance(int16_t adccode)
{
  float mvolt;
  
  mvolt = (((uint32_t)adccode)*((5.1/1023)/OPAMP_GAIN));
// 1) Uo = (5.0 / 1023.0) * adccode;
// 2) Ui = (Uo-(Vcc*Rfb/R1)) / (1 + Rfb/R2 + Rfb/R1)  
// 3) R2 = 1000 / ((5.0 / Ui) - 1); 
   
  return (int16_t)(1000UL / ((5.1 / mvolt) - 1));
}