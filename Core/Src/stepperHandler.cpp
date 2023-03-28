/*
 * stepperHandler.cpp
 *
 *  Created on: Dec 12, 2022
 *      Author: Mazur
 */
#include "stateMachine.h"
#include "stepperHandler.h"
#include "main.h"
uint32_t milisecondCounter = 0;
uint32_t stepsPerSecond = 0;
void setupStepperDriver()
{
	HAL_GPIO_WritePin(stepper_m0_GPIO_Port, stepper_m0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(stepper_m1_GPIO_Port, stepper_m1_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(stepper_m2_GPIO_Port, stepper_m2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(stepper_dir_GPIO_Port, stepper_dir_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(stepper_enable_GPIO_Port, stepper_enable_Pin, GPIO_PIN_SET);
	stepsPerSecond = 1;


}
void setStepperRate(int ratePerSecond)
{
	stepsPerSecond = ratePerSecond;
}
void setStepperEnable(GPIO_PinState enable)
{
	HAL_GPIO_WritePin(stepper_enable_GPIO_Port, stepper_enable_Pin, enable);
}
void setStepperDirection (int direction)
{
	if(direction) HAL_GPIO_WritePin(stepper_dir_GPIO_Port, stepper_dir_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(stepper_dir_GPIO_Port, stepper_dir_Pin, GPIO_PIN_RESET);

}
void handleStepsInterrupt()
{
	milisecondCounter++;
		if (milisecondCounter >= (10000.0/stepsPerSecond))
		{
			if(stepsPerSecond)
			{
				milisecondCounter = 0;
				//HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
				//HAL_GPIO_TogglePin(stepper_step_GPIO_Port, stepper_step_Pin);
				HAL_GPIO_WritePin(stepper_step_GPIO_Port, stepper_step_Pin, GPIO_PIN_RESET);
				delay(1);
				HAL_GPIO_WritePin(stepper_step_GPIO_Port, stepper_step_Pin, GPIO_PIN_SET);

				//HAL_GPIO_TogglePin(stepper_step_GPIO_Port, stepper_step_Pin);

			}
		}


	  interruptFlicker();

}

void delay (int a)
{
  volatile int i,j;
  for (i=0 ; i < a ; i++)
  {
     j++;
  }
  return;
}

