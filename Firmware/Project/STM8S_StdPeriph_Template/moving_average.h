/*
 * moving_average.h
 *
 *  Created on: 2008-12-12
 *      Author: wodz
 */

#ifndef MOVING_AVERAGE_H_
#define MOVING_AVERAGE_H_

#define MAX_WINDOW_SIZE	32

typedef struct {
	int16_t data[MAX_WINDOW_SIZE];
	uint8_t index;
	uint8_t mask;
	uint8_t init;
} movingaverage_t;

int16_t moving_average(movingaverage_t *filter, uint16_t data, uint8_t samples);

#endif /* MOVING_AVERAGE_H_ */
