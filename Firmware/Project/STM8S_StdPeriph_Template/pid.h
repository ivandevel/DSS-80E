#ifndef PID_H_
#define PID_H_

int16_t pid(uint16_t ref, int16_t inp);
int32_t PIDcal(int32_t setpoint, int32_t actual_position);

#endif /* PID_H_ */
