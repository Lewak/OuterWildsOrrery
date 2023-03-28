/*
 * button.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Mazur
 */
#include <main.h>
#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

int isButtonChanged();
int isButtonHeld();
int hasButtonBeenReleased();
int resetState();
int wasButtonHeldForPeriod(int timePeriod, uint32_t deltaTime);

#endif /* INC_BUTTON_H_ */
