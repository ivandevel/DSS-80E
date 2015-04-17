/*
 * moving_average.c
 *
 *  Created on: 2008-12-12
 *      Author: wodz
 */
#include <stdint.h>

#include "moving_average.h"

int16_t moving_average(movingaverage_t *filter, uint16_t data, uint8_t samples)
{
	uint8_t i;
	int16_t sum = 0;

	// initialization
	if (filter->init == 0)
	{
		for (i=1;i<samples;i++)
			filter->data[i] = data;

		// set the mask
		filter->mask = samples - 1;

		// set the mark that filter's data structure has been initialized
		filter->init = 1;
	}

	// insert new data
	filter->data[filter->index++ & filter->mask] = data;

	// calculate the average
	for (i=0;i<samples;i++)
		sum += filter->data[i];

	return (int16_t)(sum/samples);
}
