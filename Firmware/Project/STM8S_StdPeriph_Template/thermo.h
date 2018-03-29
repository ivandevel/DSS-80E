#ifndef THERMO_H_
#define THERMO_H_

#ifdef SOLDERING_VAR1
#define OPAMP_GAIN 1 //1.0

static const int16_t vArray[20] = 
{
0, 35,  
66, 50, 
290, 100,  
520, 150,
720, 250,
1400, 350,
1800,  450
};

#elif SOLDERING_VAR2
#define OPAMP_GAIN 1 //1.0

static const int16_t vArray[22] = 
{
0, 35,  
880, 50, 
885, 100,  
1020, 150,
1420, 250,
2250, 350,
2780,  450,
2980,  480
};

#elif SOLDERING_HAKKO_A1321_MINI
#define OPAMP_GAIN 1 //1.0
static const int16_t vArray[22] = 
{
0, 35,  
880, 50, 
885, 100,  
1020, 150,
1420, 250,
2250, 350,
2780,  450,
2980,  480
};
#elif SOLDERING_HAKKO_A1321
#define OPAMP_GAIN 1 //1.0
static const int16_t vArray[22] = 
{
0, 35,  
880, 50, 
885, 100,  
1020, 150,
1420, 250,
2250, 350,
2780,  450,
2980,  480
};
#else
 #error "Please select first the STM8S SOLDERING board to be used (in stm8s_eval.h)"
#endif

int16_t Convert(int16_t adc_code, int16_t tcj);
int16_t Code2uV(int16_t adccode);
#endif