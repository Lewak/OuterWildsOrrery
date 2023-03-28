/*
 * stateMachine.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Mazur
 */
#include "main.h"
#include <tuple>

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

typedef enum  {
	SETTINGS,
	MAINSCREEN,
	CHANGEVOLUME,
	CHANGELIGHT,
	CHANGESPEED,
	SKIPSCREEN,
	MISC
} UIStates;
typedef enum{
	DISABLED,
	FROZEN,
	NORMAL,
	ENDTIMES,
	EXPLOSION
} timeStates;

void stateHandler();
void timeHandler();
void resetTimer();
void updateTime();
void changeState(int state);
void changeTimeState(int state);
void interruptFlicker();
void setRGBoutput(uint8_t data[3]);
void transitionRGBcolor(uint8_t color1[3], uint8_t color2[3], double timeLimit, uint32_t timer);
void setupState(GPIO_PinState motorOn, uint8_t sunColor[3]);
void explodeSun();
void handleDisplayRefresh();
std::tuple<int, int>  getTime(uint32_t timer);
int translateEncoderData(int data, int maxValue, int MinValue);


#endif /* INC_STATEMACHINE_H_ */
