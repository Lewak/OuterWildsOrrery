/*
 * StepperHandler.h
 *
 *  Created on: Dec 12, 2022
 *      Author: Mazur
 */
#include "main.h"
#ifdef __cplusplus
extern "C"
{
#endif
#ifndef INC_STEPPERHANDLER_H_
#define INC_STEPPERHANDLER_H_

void setupStepperDriver();
void setStepperRate(int rate);
void setStepperDirection (int direction);
void setStepperEnable(GPIO_PinState enable);
void handleStepsInterrupt();
void delay (int a);

#ifdef __cplusplus
}
#endif




#endif /* INC_STEPPERHANDLER_H_ */
