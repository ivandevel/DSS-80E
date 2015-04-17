#ifndef THERMO_H_
#define THERMO_H_

#define OPAMP_GAIN 1 //1.0
int16_t Convert(int16_t adc_code, int16_t tcj);
int16_t Code2uV(int16_t adccode);
#endif