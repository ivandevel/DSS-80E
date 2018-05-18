#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "pid.h"
#include <math.h>
#include <inttypes.h>

#ifdef DFS_90

#define POW_MAX 1000
#define POW_MIN 0
#define PidH_Kp   20  
#define PidH_Ki   22  
#define PidH_Kd   20  
#define TSAMPLE   100

#else

#define POW_MAX 90
#define POW_MIN 11
#define PidH_Kp   20  
#define PidH_Ki   22  
#define PidH_Kd   20  
#define TSAMPLE   100

#endif

#pragma optimize=none
int16_t pid(uint16_t ref, int16_t inp)
{
  static int Xp = 0;  //предыдущее значение inp
  static int Xpp = 0; //пред-предыдущее значение inp
  static long Y = 0;
  //вычисление ошибки:
  int e = ref - inp;
  //учет пропорциональной составл€ющей:
  Y = Y - (long)PidH_Kp * (inp - Xp) * 10;
  //учет интегральной составл€ющей:
  Y = Y + (long)PidH_Ki * e * TSAMPLE / 100;
  //учет дифференциальной составл€ющей:
  Y = Y - (long)PidH_Kd * (inp - 2 * Xp + Xpp) * 500 / TSAMPLE;
  //обновление значений:
  Xpp = Xp; Xp = inp;
  if(!ref) Y = 0;
  //if(!PidH_En) Y = (long)out << 8;
  //ограничение выходного значени€:
	if(Y > POW_MAX * 256L) 
          Y = POW_MAX * 256L;
	if(Y < POW_MIN * 256L) 
          Y = POW_MIN * 256L;
	return(Y >> 8);
}
