#include "stm8s.h"
#include "thermo.h"

static const int16_t vArray[20] = 
{
0, 35,  
66, 50, 
290, 100,  
520, 150,
970, 250,
1400, 350,
1800,  450
};

//#pragma optimize=none
int16_t Convert(int16_t adc_code, int16_t tcj) 
{
uint8_t x;
uint16_t Vread = Code2uV(adc_code);

for (x = 0; x < 12; x+=2)
{
 if (Vread >=vArray[x] && Vread <= vArray[x+2]) 
 {
   return vArray[x+1] + (int16_t)((int32_t)(vArray[x+3] - vArray[x+1]) * (int32_t)(Vread - vArray[x]) / (int32_t)(vArray[x+2] - vArray[x]));  
 }
}
return 777;
}

int16_t Code2uV(int16_t adccode)
{
  //5350 - коэффициент усилителя 535.0
  //1023 - разрешающая способность АЦП
  //510000000 - опорное напряжение АЦП 5 Вольт
  return (((uint32_t)adccode)*((5100000UL/1023UL)/OPAMP_GAIN))/1000;
}
