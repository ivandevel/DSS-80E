#ifndef PID_H_
#define PID_H_

#define MASK 0x01				//bit mask to access current error in errors array
#define PID_TOP	47//513				//max value regulator function should return
#define PID_BOTTOM 0			//min value regulator function should return
#define MAX_PID_CONSTANT 32		//max value of constants to avoid overflows

typedef struct {
	uint8_t KP;				//proportional term constant 0-32
	uint8_t KI;				//integral term constant 0-32
	uint8_t KD;				//differential term constant 0-32
	uint8_t KT;				//threshold between PD and PID behavior of regulator function
	uint8_t index;				//errors array index
	int16_t errors[2];			//errors array
	int16_t integral;			//integral of the error
	int16_t derivative;			//derivative of the error
} pid_t;

int16_t pid(uint16_t setpoint, uint16_t temperature, pid_t *pid_setup);
int32_t PIDcal(int32_t setpoint,int32_t actual_position);
#endif /* PID_H_ */
