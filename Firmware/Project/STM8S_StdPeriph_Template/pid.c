#include <stdint.h>
#include <stdio.h>
#include "pid.h"

int16_t pid(uint16_t setpoint, uint16_t temperature, pid_t *pid_s)
{
	int32_t result;

	// calculate regulator error
	pid_s->errors[pid_s->index & MASK] = setpoint - temperature;

	// calculate derivative term
	pid_s->derivative = pid_s->errors[pid_s->index & MASK] - pid_s->errors[(pid_s->index-1) & MASK];

	// if we are far away from setpoint use PD otherwise use full PID function
	if (pid_s->errors[pid_s->index & MASK] > pid_s->KT || pid_s->errors[pid_s->index & MASK] < -pid_s->KT)
	{
		result = (pid_s->KP*pid_s->errors[pid_s->index & MASK] + pid_s->KD*pid_s->derivative);
	}
	else
	{
		// integrate error
		pid_s->integral += pid_s->errors[pid_s->index & MASK];

		// PID function
		result = (pid_s->KP*pid_s->errors[pid_s->index & MASK] + (pid_s->KI*pid_s->integral)/100 + pid_s->KD*pid_s->derivative);
	}

	pid_s->index++;

	// regulator function bandwidth set to PID_TOP - PID_BOTTOM
	if (result > PID_TOP)
	{
		return (int16_t)PID_TOP;
	}
	else if (result < PID_BOTTOM)
	{
		return (int16_t)PID_BOTTOM;
	}
	else
	{
		return (int16_t)result;
	}


}
/*
//Define parameter
#define epsilon 5
#define dt 3             //100ms loop time
#define MAX  512                   //For Current Saturation
#define MIN 0
#define Kp  25
#define Kd  25
#define Ki  2

int32_t PIDcal(int32_t setpoint, int32_t actual_position)
{
	static int32_t pre_error = 0;
	static int32_t integral = 0;
	int32_t error;
	int32_t derivative;
	int32_t output;

	//Caculate P,I,D
	error = setpoint - actual_position;

	//In case of error too small then stop intergration
	if(abs(error) > epsilon)
	{
		integral = integral + error/dt;
	}
	derivative = (error - pre_error)/dt;
	output = Kp*error + Ki*integral + Kd*derivative;

	//Saturation Filter
	if(output > MAX)
	{
		output = MAX;
	}
	else if(output < MIN)
	{
		output = MIN;
	}
        //Update error
        pre_error = error;

 return output;
}
*/