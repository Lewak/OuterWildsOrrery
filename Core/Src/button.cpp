/*
 * button.cpp
 *
 *  Created on: Dec 15, 2022
 *      Author: Mazur
 */
#include <bitmap.h>
#include <button.h>
#include "logic.h"
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "ARGB.h"
#include "libs.h"
#include "ssd1306.h"
#include "DFPLAYER_MINI.h"
#include "songs.h"
#include "stepperHandler.h"
int currentValue = 0;
int previousValue = 1;
int didButtonStateChanged = 0;
int triggeredHeld = 0;
uint32_t lastButtonTime = 0;
uint16_t buttonTimer = 0;
int hasButtonChanged()
{
	int temp = HAL_GPIO_ReadPin(encoder_button_GPIO_Port, encoder_button_Pin);
	if (temp == previousValue)
	{
		return 0;
	}
	previousValue = temp;
	didButtonStateChanged = 1;
	return temp;
}
int isButtonHeld()
{
	return !HAL_GPIO_ReadPin(encoder_button_GPIO_Port, encoder_button_Pin);

}
int hasButtonBeenReleased()
{
	int temp = HAL_GPIO_ReadPin(encoder_button_GPIO_Port, encoder_button_Pin);

	if ((temp == 1) and (previousValue == 0))
	{
		previousValue = temp;

			if(!triggeredHeld){
				didButtonStateChanged = 1;
				return 1;
			}
			else{
				didButtonStateChanged = 1;
				triggeredHeld = 0;
				return 0;

			}

	}
	previousValue = temp;

	return 0;
}
int wasButtonStateChanged()
{
	int temp = didButtonStateChanged;
	didButtonStateChanged = 0;
	return temp;
}
int wasButtonHeldForPeriod(int timePeriod, uint32_t deltaTime)
{
	if(isButtonHeld()) buttonTimer += deltaTime - lastButtonTime;
	else buttonTimer = 0;
	lastButtonTime = deltaTime;
	if(buttonTimer >= timePeriod and !triggeredHeld)
	{
		buttonTimer = 0;
		triggeredHeld = 1;
		return 1;
	}
	return 0;

}
