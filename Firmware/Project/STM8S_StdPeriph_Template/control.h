//----------------------------------------------------------------------------

//DSS-90/DFS-90
//модуль управления нагревом и потоком воздуха: заголовочный файл

//----------------------------------------------------------------------------

#ifndef ControlH
#define ControlH

//------------------------------ Константы: ----------------------------------
#define TSAMPLE     50
#define THJ_MAX     450
#define T_MIN         0  //минимальная температура, x0.1°C
#define T_MAX   THJ_MAX  //максимальная температура, x0.1°C
#define V_MIN         0  //минимальная скорость, rpm
#define V_MAX      9999  //максимальная скорость, rpm
#define COMP_MIN      0  //минимальная  компенсация
#define COMP_MAX      9  //максимальная компенсация
#define DELAY_MAX 30000  //максимальное значение задержек, мс

#define POW_MIN 0
#define POW_MAX 48
//Коды ошибок:
enum
{
  ER_NO,   //нет ошибки
  ER_FAN,  //ошибка вентилятора
  ER_HEAT, //ошибка нагревателя
  ER_OVER, //ошибка перегрева
  ER_SENS  //ошибка сенсора
};

//-------------------------- Прототипы функций: ------------------------------

void Control_Init(void);      //инициализация модуля
void Control_Exe(void);     //управление нагревом

char Control_MainsOn(void);   //чтение подключения к сети
char Control_GetError(void);  //чтение кода ошибки

char Control_GetTupd(void);   //чтение флага обновления температуры
int  Control_GetTc(void);     //чтение текущей температуры, x0.1°C
void Control_SetT(int t);     //установка температуры, x0.1°C
int  Control_GetT(void);      //чтение установленной температуры, x0.1°C
void Control_SetP(int p);     //установка мощности, x0.1%
int  Control_GetP(void);      //чтение текущей мощности, x0.1%
void Control_SetPidH(char kp, char ki, char kd); //уст. коэфф. PIDH
void Control_GetPidH(char *kp, char *ki, char *kd); //чтение коэфф. PIDH
void Control_SetEnH(char e);  //разрешение PIDH
char Control_GetEnH(void);    //чтение разрешения PIDH
void Control_SetErrH(char enh, char eno, char ens,
                     int tm, int dt, int vt); //установка параметров ErH
void Control_GetErrH(char *enh, char *eno, char *ens,
                     int *tm, int *dt, int *vt); //чтение параметров ErH
void Control_SetPlim(int p);  //установка максимальной мощности, x0.1%
int  Control_GetPlim(void);   //чтение максимальной мощности, x0.1%
void Control_SetComp(char c); //установка компенсации
char Control_GetComp(void);   //чтение компенсации
void Control_SetTc(int tTemp);
//----------------------------------------------------------------------------

#endif
