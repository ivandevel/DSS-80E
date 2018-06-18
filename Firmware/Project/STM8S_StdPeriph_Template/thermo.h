#ifndef THERMO_H_
#define THERMO_H_

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

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

static const int16_t vArray[] = 
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
static const int16_t vArray[] = 
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
#define OPAMP_GAIN 900 //1.0
static const int16_t vArray[] = 
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
#elif DFS_90
#define OPAMP_GAIN 100 //1.0

static const int16_t vArray[] = 
{
397, 10,
798, 20,
1203, 30,
1612, 40,
2023, 50,
2436, 60,
2851, 70,
3267, 80,
3682, 90,
4096, 100,
4920, 120,
6138, 150, 
6941, 170,
8138, 200, 
8940, 220,
10153, 250,
10971, 270,
12209, 300,
14293, 350,
15133, 370,
16397, 400,
20644, 500,
21924, 530,
22350, 540,
22776, 550,
23203, 560,
23629, 570,
24055, 580,
24480, 590,
24905, 600
};
#else
 #error "Please select first the STM8S SOLDERING board to be used (in stm8s_eval.h)"
#endif

int16_t Convert(int16_t adc_code, int16_t tcj);
int16_t Code2uV(int16_t adccode);
#endif