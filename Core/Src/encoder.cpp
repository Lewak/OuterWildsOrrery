/*
 * encoder.c
 *
 *  Created on: Dec 16, 2022
 *      Author: Mazur
 */
#include "encoder.h"
#include "main.h"

int16_t previousEncoderValue = 0;

int wasEncoderMoved()
{
	if(getEncoderChange() != previousEncoderValue)
	{
		previousEncoderValue = getEncoderChange();
		return 1;
	}
	else
	{
		previousEncoderValue = getEncoderChange();
		return 0;
	}
}
int getAbsoluteEncoderValue()
{
	return (TIM2->CNT)>>2;
}

int getEncoderChange()
{
	return (static_cast<int16_t>(TIM2->CNT)>>2);
}
