//----------------------------------------------------------------------------

//DSS-90/DFS-90
//модуль управления нагревом и потоком воздуха

//Для поддержания температуры используется PID-алгоритм. Закон PID
//записан в дифференциальной форме (еще эту форму называют инкрементальной).
//Такая запись получается дифференцированием обычной формулы. Вычисляется
//дифференциал выходного воздействия, само воздействие получается
//накапливающим суммированием.
//При таком способе вычисления PID отсутствует необходимост ограничения
//интегральной составляющей. Ограничивать нужно лишь полученное
//выходное воздействие, для чего есть четкие критерии.
//Желаемая температура определяется переменной Tset, на ее основе
//с учетом измеренной температуры Tget формируется необходимое значение
//мощности нагревателя Pact. Сверху мощность ограничена значением Plim.
//Имеется возможность непосредственного управления мощностью нагревателя,
//минуя PID. Для этого нужно установить PidH_Ki = 0. В таком режиме
//мощность определяется переменной Pset.

//Для DFS:
//Для поддержания частоты вращения ротора вентилятора также
//используется PID-алгоритм. Желаемая скорость вентилятора определяется
//переменной Vset, на ее основе с учетом измеренной скорости Vget
//формируется необходимое значение PWM вентилятора Wact.
//Имеется возможность непосредственного управления PWM вентилятора,
//минуя PID. Для этого нужно установить PidF_Ki = 0. В таком режиме
//PWM определяется переменной Wset.

//Коэффициенты PID и значение максимальной мощности при их задании
//сохраняются в EEPROM и считываются оттуда каждый раз при старте модуля.

//Модуль формирует коды ошибок при неисправности одной из подсистем.
//Код ошибки может быть считан с помощью функции Control_GetError().

//----------------------------------------------------------------------------
#include "stm8s.h"
#include "control.h"
//------------------------------ Константы: ----------------------------------

#define SPS 4//(1000 / TSAMPLE) //количество отсчетов температуры в секунду

#define P_LIM_NOM     47  //номинальная мощность ограничения, x0.1%
#define PIDH_KP_NOM   20  //номинальный коэффициент PidH_Kp
#define PIDH_KI_NOM   22  //номинальный коэффициент PidH_Ki
#define PIDH_KD_NOM   20  //номинальный коэффициент PidH_Kd

#define OPT_ERHTM_NOM  3000  //интервал диагностики нагревателя, мс
#define OPT_ERHDT_NOM   350  //максимальное отклонение температуры, x0.1°C
#define OPT_ERHVT_NOM    70  //макс. скорость роста температуры, x0.1°C/с

#define PIDF_KP_NOM      47  //номинальный коэффициент PidF_Kp
#define PIDF_KI_NOM      70  //номинальный коэффициент PidF_Ki
#define PIDF_KD_NOM      50  //номинальный коэффициент PidF_Kd

#define OPT_ERFTM_NOM  5000  //интервал диагностики вентилятора, мс
#define OPT_ERFDV_NOM   750  //максимальное отклонение скорости, rpm

#define ECSIGN       0xBEDD  //сигнатура EEPROM

//----------------------------- Переменные: ----------------------------------

static char Error;        //текущий код ошибки
static char ErrorH;       //код ошибки нагревателя
static char DiagTimerH;   //таймер диагностики нагревателя
static int  Temp[SPS];    //массив температур для дифференцирования
static char TempPnt;      //указатель массива температур

static char Tupd;         //флаг обновления температуры
static int  Tget;         //измеренное значение температуры, x0.1°C
static int  Tset;         //установленное значение температуры, x0.1°C
static int  Pact;         //текущее значение мощности нагревателя, x0.1%
static int  Plim;         //ограничение мощности нагревателя, x0.1%
static int  Pset;         //заданная мощность нагр. (для работы без PID), x0.1%
static char PidH_Kp;      //Kp PIDH
static char PidH_Ki;      //Ki PIDH
static char PidH_Kd;      //Kd PIDH
static char PidH_En;      //разрешение PID нагревателя
static char OptErHen;     //разрешение обработки ErH
static char OptErOen;     //разрешение обработки ErO
static char OptErSen;     //разрешение обработки ErS
static int  OptErHTm;     //задержка для ErH
static int  OptErHdT;     //ошибка температуры для ErH
static int  OptErHvT;     //скорость изменения температуры для ErH

 //int  ECSign;    //сигнатура EEPROM
char EPidH_Kp;  //Kp PIDH в EEPROM
 char EPidH_Ki;  //Ki PIDH в EEPROM
 char EPidH_Kd;  //Kd PIDH в EEPROM
 //int  EPlim;     //Plim в EEPROM
 char EOptErHen; //разрешение ErH в EEPROM
 char EOptErOen; //разрешение ErO в EEPROM
 char EOptErSen; //разрешение ErS в EEPROM
 int  EOptErHTm; //задержка для ErH в EEPROM
 int  EOptErHdT; //ошибка температуры для ErH в EEPROM
 int  EOptErHvT; //скорость изм. темп. для ErH в EEPROM


//-------------------------- Прототипы функций: ------------------------------

int PidH(int ref, int inp, int out);   //PID-регулятор нагревателя
#ifdef DFS
  int PidF(int ref, int inp, int out); //PID-регулятор вентилятора
#endif

//------------------------ Инициализация модуля: -----------------------------

void Control_Init(void)
{
  //чтение EEPROM:
    Control_SetPidH(PIDH_KP_NOM, PIDH_KI_NOM, PIDH_KD_NOM);
    Control_SetPlim(P_LIM_NOM);
    Control_SetErrH(1, 1, 1,
                    OPT_ERHTM_NOM, OPT_ERHDT_NOM, OPT_ERHVT_NOM);
  

  //инициализация переменных:
  Error = ER_NO;

  Tset = 0;
  Pset = 0;
  Pact = 0;
  PidH_En = 1;
  TempPnt = 0;
  ErrorH = ER_NO;
}

//-------------------- Реализация общего управления: -------------------------

void Control_Exe(void)
{
    int dT = Tget - Temp[TempPnt];   //вычисление дифференциала температуры 
    Temp[TempPnt++] = Tget;          //сохр. температуры в кольцевом буфере
    if(TempPnt == SPS) TempPnt = 0;  //коррекция указателя

    Pact = PidH(Tset, Tget, Pset);   //расчет мощности по PID-закону
    if(Pact > Plim) Pact = Plim;     //ограничение максимальной мощности:
    
    if (Pact < 0) 
      Pact = 0;
    
    ErrorH = ER_NO;                  //сброс кода ошибки
    if(DiagTimerH) DiagTimerH--;     //таймер диагностики нагревателя
    else                             //если время уст. температуры истекло,
    {
      if(OptErHen &&                 //если обработка ошибки ErH разрешена
         ((Tset - Tget) > OptErHdT) &&  //и температура ниже заданной на DT
         (dT < OptErHvT))               //и скорость роста меньше VT,
        ErrorH = ER_HEAT;               //то ошибка нагревателя
      if(OptErOen &&                 //если обработка ошибки ErO разрешена
         ((Tget - Tset) > OptErHdT) &&  //если температура выше заданной на DT
         (dT > OptErHvT))               //и скорость роста больше VT,
        ErrorH = ER_OVER;               //то ошибка перегрева
    }
    if(OptErSen &&                   //если обработка ошибки ErS разрешена
       (Tget > THJ_MAX))             //если температура выше THJ_MAX,
      ErrorH = ER_SENS;              //то ошибка сенсора
 

    Error = ErrorH;                  //результирующая ошибка для DSS - ErrorH

}

//---------------------- PID-регулятор нагревателя: --------------------------

//Параметры регулятора:
//Pid_Kp - коэффициент пропорциональный
//Pid_Ki - коэффициент интегральный 
//Pid_Kd - коэффициент дифференциальный
//ref     - установленное значение
//inp (X) - измеренное значение
//out (Y) - воздействие

int PidH(int ref, int inp, int out)
{
  static int Xp = 0;  //предыдущее значение inp
  static int Xpp = 0; //пред-предыдущее значение inp
  static long Y = 0;
  //вычисление ошибки:
  int e = ref - inp;
  //учет пропорциональной составляющей:
  Y = Y - (long)PidH_Kp * (inp - Xp) * 10;
  //учет интегральной составляющей:
  Y = Y + (long)PidH_Ki * e * TSAMPLE / 100;
  //учет дифференциальной составляющей:
  Y = Y - (long)PidH_Kd * (inp - 2 * Xp + Xpp) * 500 / TSAMPLE;
  //обновление значений:
  Xpp = Xp; Xp = inp;
  if(!ref) Y = 0;
  if(!PidH_En) Y = (long)out << 8;
  //ограничение выходного значения:
  if(Y > POW_MAX * 256L) Y = POW_MAX * 256L;
  if(Y < POW_MIN * 256L) 
    Y = POW_MIN * 256L;
  return(Y >> 8);
}

//----------------------- PID-регулятор венилятора: --------------------------

//Параметры регулятора:
//Pid_Kp - коэффициент пропорциональный
//Pid_Ki - коэффициент интегральный 
//Pid_Kd - коэффициент дифференциальный
//ref     - установленное значение
//inp (X) - измеренное значение
//out (Y) - воздействие

#ifdef DFS
int PidF(int ref, int inp, int out)
{
  static int Xp = 0;  //предыдущее значение inp
  static int Xpp = 0; //пред-предыдущее значение inp
  static long Y = 0;
  //вычисляем ошибку:
  int e = ref - inp;
  //учет пропорциональной составляющей:
  Y = Y - (long)PidF_Kp * (inp - Xp) / 10;
  //учет интегральной составляющей:
  Y = Y + (long)PidF_Ki * e * VSAMPLE / 10000;
  //учет дифференциальной составляющей:
  Y = Y - (long)PidF_Kd * (inp - 2 * Xp + Xpp) / VSAMPLE;
  //обновление значений:
  Xpp = Xp; Xp = inp;
  if(!ref) Y = 0;
  if(!PidF_En) Y = (long)out << 8;
  //ограничение выходного значения:
  if(Y > PWM_MAX * 256L) Y = PWM_MAX * 256L;
  if(Y < PWM_MIN * 256L) Y = PWM_MIN * 256L;
  return(Y >> 8);
}
#endif

//----------------------------------------------------------------------------
//-------------------------- Интерфейсные функции: ---------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//--------------------------------- Общие: -----------------------------------
//----------------------------------------------------------------------------

//------------------------ Чтение подключения к сети: ------------------------

char Control_MainsOn(void)
{
  return(0);
}

//--------------------------- Чтение кода ошибки: ----------------------------

char Control_GetError(void)
{
  return(Error);
}

//----------------------------------------------------------------------------
//------------------------------ Нагреватель: --------------------------------
//----------------------------------------------------------------------------

//------------------ Чтение флага обновления температуры: --------------------

char Control_GetTupd(void)
{
  return(Tupd);
}

//---------------------- Чтение измеренной температуры: ----------------------

int Control_GetTc(void)
{
  return(Tget);
}

//---------------------- Задание измеренной температуры: ----------------------

void Control_SetTc(int tTemp)
{
  Tget = tTemp ;
}

//------------------------- Установка температуры: ---------------------------

void Control_SetT(int t)
{
  if(t < T_MIN) t = T_MIN;         //ограничение температуры
  if(t > T_MAX) t = T_MAX;
  Tset = t;                        //установка температуры
  DiagTimerH = OptErHTm / TSAMPLE; //перезагрузка таймера диагностики
}

//----------------------- Чтение заданной температуры: -----------------------

int Control_GetT(void)
{
  return(Tset);
}

//--------------------------- Установка мощности: ----------------------------

void Control_SetP(int p)
{
  if(p < POW_MIN) p = POW_MIN;
  if(p > POW_MAX) p = POW_MAX;
  Pset = p;
}

//------------------------ Чтение текущей мощности: --------------------------

int Control_GetP(void)
{
  return(Pact);
}

//--------------------- Установка коэффициентов PIDH: ------------------------

void Control_SetPidH(char kp, char ki, char kd)
{
  PidH_Kp = kp; if(EPidH_Kp != kp) EPidH_Kp = kp;
  PidH_Ki = ki; if(EPidH_Ki != ki) EPidH_Ki = ki;
  PidH_Kd = kd; if(EPidH_Kd != kd) EPidH_Kd = kd;
}

//---------------------- Чтение коэффициентов PIDH: --------------------------

void Control_GetPidH(char *kp, char *ki, char *kd)
{
  *kp = PidH_Kp; *ki = PidH_Ki; *kd = PidH_Kd;
}

//--------------------------- Разрешение PIDH: -------------------------------

void Control_SetEnH(char e)
{
  PidH_En = e;
}

//------------------------- Чтение разрешения PIDH: --------------------------

char Control_GetEnH(void)
{
  return(PidH_En);
}

//-------------------- Установка параметров ошибки ErH: ----------------------

void Control_SetErrH(char enh, char eno, char ens, int tm, int dt, int vt)
{
  if(tm < 0) tm = 0; if(tm > DELAY_MAX) tm = DELAY_MAX;
  if(dt < 0) dt = 0; if(dt > T_MAX) dt = T_MAX;
  if(vt < 0) vt = 0; if(vt > T_MAX) vt = T_MAX;
  OptErHen = enh; if(EOptErHen != enh) EOptErHen = enh;
  OptErOen = eno; if(EOptErOen != eno) EOptErOen = eno;
  OptErSen = ens; if(EOptErSen != ens) EOptErSen = ens;
  OptErHTm = tm; if(EOptErHTm != tm) EOptErHTm = tm;
  OptErHdT = dt; if(EOptErHdT != dt) EOptErHdT = dt;
  OptErHvT = vt; if(EOptErHvT != vt) EOptErHvT = vt;
}

//--------------------- Чтение параметров ошибки ErH: ------------------------

void Control_GetErrH(char *enh, char *eno, char *ens, int *tm, int *dt, int *vt)
{
  *enh = OptErHen; *eno = OptErOen; *ens = OptErSen;
  *tm = OptErHTm; *dt = OptErHdT; *vt = OptErHvT;
}

//-------------------- Установка максимальной мощности: ----------------------

void Control_SetPlim(int p)
{
  if(p < POW_MIN) p = POW_MIN;
  if(p > POW_MAX) p = POW_MAX;
  Plim = p;
  //if(EPlim != p) EPlim = p;
}

//---------------------- Чтение максимальной мощности: -----------------------

int Control_GetPlim(void)
{
  return(Plim);
}

//--------------------- Установка компенсации (dummy): -----------------------

void Control_SetComp(char c)
{
}

//---------------------- Чтение компенсации (dummy): -------------------------

char Control_GetComp(void)
{
  return(0);
}

//----------------------------------------------------------------------------
//------------------------------ Вентилятор: ---------------------------------
//----------------------------------------------------------------------------

#ifdef DFS

//-------------------- Чтение флага обновления скорости: ---------------------

char Control_GetVupd(void)
{
  return(Vupd);
}

//----------------------- Чтение измеренной скорости: ------------------------

int Control_GetVc(void)
{
  return(Vget);
}

//-------------------------- Установка скорости: -----------------------------

void Control_SetV(int v)
{
  if(v < V_MIN) v = V_MIN;         //ограничение скорости
  if(v > V_MAX) v = V_MAX;
  Vset = v;                        //установка скорости
  DiagTimerF = OptErFTm / VSAMPLE; //то перезагрузка таймера диагностики
}

//------------------------ Чтение заданной скорости: -------------------------

int Control_GetV(void)
{
  return(Vset);
}

//------------------- Установка значения PWM вентилятора: --------------------

void Control_SetW(char w)
{
  Wset = w;
}

//---------------- Чтение текущего значения PWM вентилятора: -----------------

char Control_GetW(void)
{
  return(Wact);
}

//--------------------- Установка коэффициентов PIDF: ------------------------

void Control_SetPidF(char kp, char ki, char kd)
{
  PidF_Kp = kp; if(EPidF_Kp != kp) EPidF_Kp = kp;
  PidF_Ki = ki; if(EPidF_Ki != ki) EPidF_Ki = ki;
  PidF_Kd = kd; if(EPidF_Kd != kd) EPidF_Kd = kd;
}

//---------------------- Чтение коэффициентов PIDF: --------------------------

void Control_GetPidF(char *kp, char *ki, char *kd)
{
  *kp = PidF_Kp; *ki = PidF_Ki; *kd = PidF_Kd;
}

//--------------------------- Разрешение PIDF: -------------------------------

void Control_SetEnF(char e)
{
  PidF_En = e;
}

//------------------------- Чтение разрешения PIDF: --------------------------

char Control_GetEnF(void)
{
  return(PidF_En);
}

//-------------------- Установка параметров ошибки ErF: ----------------------

void Control_SetErrF(char en, int tm, int dv)
{
  if(tm < 0) tm = 0; if(tm > DELAY_MAX) tm = DELAY_MAX;
  if(dv < 0) dv = 0; if(dv > V_MAX) dv = V_MAX;
  OptErFen = en; if(EOptErFen != en) EOptErFen = en;
  OptErFTm = tm; if(EOptErFTm != tm) EOptErFTm = tm;
  OptErFdV = dv; if(EOptErFdV != dv) EOptErFdV = dv;
}

//--------------------- Чтение параметров ошибки ErF: ------------------------

void Control_GetErrF(char *en, int *tm, int *dv)
{
  *en = OptErFen; *tm = OptErFTm; *dv = OptErFdV;
}

#endif

//----------------------------------------------------------------------------
