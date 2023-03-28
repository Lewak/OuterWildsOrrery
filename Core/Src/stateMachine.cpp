/*
 *
 * stateMachine.cpp
 *
 *  Created on: Dec 15, 2022
 *      Author: Mazur
 */

#include <bitmap.h>
#import "stateMachine.h"
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
#include "button.h"
#include "encoder.h"
#include <tuple>

uint32_t mainTimer = 0;
uint32_t realTimer = 0;
uint32_t lastTime = 0;
//uint32_t clockTimer = 0;
int currentState = MAINSCREEN;
int nextState = MAINSCREEN;
int currentTimeState = DISABLED;
int nextTimeState = DISABLED;
int wasStateChanged = 1;
int wasTimeStateChanged = 1;
float timeModifier = 1.0;
int toggleScreen = 0;
u16_t refreshTimer = 0;
int wasDisplayDataChanged = 1;
int lastSecond = 0;
int lastMinute = 0;
u8_t RGBdata[3] = {0,0,0};
static u8_t sunOff[3] = {0,0,0};
static u8_t healthySun[3] = {255,137,0};
static u8_t redSun[3] = {255,0,0};
static u8_t explosionRGB[8][3] = {{255,0,0},{50,5,5},{20,5,5},{0,0,0},{0,0,255},{100,100,255},{0,0,0},{0,0,0}};
static u16_t explosionTimes[7] = {5300,1500,300,100,25000,10000,10000};
static u16_t buttonHeldTime = 1000;
u8_t explosionIterator = 0;
int flickerEnable = 0;
static u16_t fadeoutTime = 4000;
u16_t encoderPoint = 0;
int volume = 25;
int previousVolume = 25;
u8_t lightLevel = 255;
int lightSetting = 30;
int previousLightSetting = 30;
int speedSetting = 15;
int previousSpeedSetting = 15;
int skipSetting = 0;
int previousSkipSetting = 0;
static double solarTimeLimit = 1000 * 60 * 21;
char* settingNames[6] = { "BACK", "VOLM", "LGHT", "SPED", "SKIP", "MISC"};
char* miscNames[8] = {"BACK", "FLIK", "ETER", "DIRE", "PAUS", "REPE", "PICT", "SOFF"};
int miscFlicker = 1;
int miscEternal = 0;
int miscDirection = 1;;
int miscPicMode = 1;
int miscScreenOff = 0;
int miscRepeat = 0;
int miscVal[8] = {0, 1, 0, 1, 0, 0, 1, 0};
int settingsIterator = 0;
int miscIterator = 0;
int miscPause = 0;

void stateHandler()
{
    std::tuple <int,int> timeValue;
    int tempSetting = 0;
	if (wasEncoderMoved()) wasDisplayDataChanged = 1;

	timeHandler();

	switch(currentState)
	{

		case MAINSCREEN:
			if(wasStateChanged)
			{
				wasStateChanged = 0;
				ssd1306_Fill(Black);
				wasDisplayDataChanged = 1;
				lastMinute = 999;
			}
			if (hasButtonBeenReleased()) changeState(SETTINGS);
		    char result[50];
			if(!miscScreenOff)
			{
		    switch(currentTimeState)
		    {
		    case DISABLED:
		    	if(!miscPicMode)
		    	{
					ssd1306_Fill(Black);
				    ssd1306_SetCursor(25,25);
			    	ssd1306_WriteString("READY", Font_16x26, White);
		    	}
		    	else
		    	{
		    		drawLogo();
		    	}

		    break;
		    case FROZEN:
		    	if(!miscPicMode)
		    	{
					ssd1306_Fill(Black);
				    ssd1306_SetCursor(10,25);
				    ssd1306_WriteString("ETERNAL", Font_16x26, White);
		    	}
		    	else
		    	{
		    		drawLogo();
		    	}
		    break;
		    case NORMAL:

		    		if(!miscPicMode)
		    		{
					    timeValue = getTime(mainTimer);
					    sprintf(result, "%02d", std::get<0>(timeValue));
						ssd1306_Fill(Black);
					    ssd1306_SetCursor(25,25);
					    ssd1306_WriteString(result, Font_16x26, White);
					    ssd1306_WriteString(":", Font_16x26, White);
					    sprintf(result, "%02d", std::get<1>(timeValue));
					    ssd1306_WriteString(result, Font_16x26, White);
					    if(lastSecond!=std::get<1>(timeValue))
					    {
					    	lastSecond=std::get<1>(timeValue);
					    	wasDisplayDataChanged = 1;
					    }

		    		}
		    		else
		    		{
					    timeValue = getTime(mainTimer);
					    if(lastMinute!=std::get<0>(timeValue))
					    {
					    	lastMinute=std::get<0>(timeValue);
					    	drawNextBitmap();
					    	wasDisplayDataChanged = 1;

					    }
		    		}


		    break;
		    case ENDTIMES:
		    	if(!miscPicMode)
		    	{
					ssd1306_Fill(Black);
				    ssd1306_SetCursor(0,25);
				    ssd1306_WriteString("END TIME", Font_16x26, White);
		    	}
		    	else
		    	{
		    		drawLogo();
		    	}
		    break;
		    case EXPLOSION:

		    	if(!miscPicMode)
		    	{
					ssd1306_Fill(Black);
				    ssd1306_SetCursor(0,25);
				    ssd1306_WriteString("THE END", Font_16x26, White);
		    	}
		    	else
		    	{
		    		drawLogo();
		    	}
		    break;

		    }

		    }
			else
			{
				ssd1306_Fill(Black);

			}


		break;
		case SETTINGS:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				settingsIterator = 0;
				wasDisplayDataChanged = 1;

			}
			if (hasButtonBeenReleased()) changeState(MAINSCREEN);

			settingsIterator = translateEncoderData(settingsIterator, 5, 0);
			ssd1306_Fill(Black);
		    ssd1306_SetCursor(0,25);
		    if(settingsIterator != 0) ssd1306_WriteString("<", Font_16x26, White);
		    ssd1306_SetCursor(30,25);

		    ssd1306_WriteString(settingNames[settingsIterator], Font_16x26, White);
		    ssd1306_SetCursor(110,25);
		    if(settingsIterator != 5) ssd1306_WriteString(">", Font_16x26, White);

			if (hasButtonBeenReleased()) changeState(settingsIterator + 1);



		break;
		case CHANGEVOLUME:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				previousVolume = volume +1;
				wasDisplayDataChanged = 1;

			}
			if (hasButtonBeenReleased()) changeState(SETTINGS);
			volume = translateEncoderData(volume, 30, 0);
			if(volume != previousVolume)
			{

				previousVolume = volume;
				DF_SetVolume(volume);
			    char result[50];
			    sprintf(result, "%i", volume);
				ssd1306_Fill(Black);
			    ssd1306_SetCursor(0,25);
			    ssd1306_WriteString("VOL: ", Font_16x26, White);
			    ssd1306_WriteString(result, Font_16x26, White);
				encoderPoint = getEncoderChange();

			}

		break;
		case CHANGELIGHT:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				previousLightSetting = lightSetting +1;
				wasDisplayDataChanged = 1;
			}

			if (hasButtonBeenReleased()) changeState(SETTINGS);
			lightSetting = translateEncoderData(lightSetting, 30, 0);
			if(lightSetting != previousLightSetting)
			{

				previousLightSetting = lightSetting;
			    char result[50];
			    sprintf(result, "%i", lightSetting);
				ssd1306_Fill(Black);
			    ssd1306_SetCursor(0,25);
			    ssd1306_WriteString("LGT: ", Font_16x26, White);
			    ssd1306_WriteString(result, Font_16x26, White);
				encoderPoint = getEncoderChange();
				lightLevel = lightSetting*255/30.0;

			}

		break;
		case CHANGESPEED:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				previousSpeedSetting = speedSetting + 1;
				wasDisplayDataChanged = 1;
			}

			if (hasButtonBeenReleased()) changeState(SETTINGS);
			speedSetting = translateEncoderData(speedSetting, 30, 0);
			if(speedSetting != previousSpeedSetting)
			{

				previousSpeedSetting = speedSetting;
			    char result[50];
			    sprintf(result, "%3d", (int)((speedSetting/30.0)*200));
				ssd1306_Fill(Black);
			    ssd1306_SetCursor(0,25);
			    ssd1306_WriteString("SPD:", Font_16x26, White);
			    ssd1306_WriteString(result, Font_16x26, White);
			    ssd1306_WriteString("%", Font_16x26, White);
				setStepperRate(speedSetting*1000/15.0);
				timeModifier = speedSetting/15.0;
				encoderPoint = getEncoderChange();
			}

		break;
		case SKIPSCREEN:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				previousSkipSetting = skipSetting + 1;
				wasDisplayDataChanged = 1;

			}
			if (hasButtonBeenReleased()) changeState(SETTINGS);

			skipSetting = (float)(mainTimer/solarTimeLimit) * 100;
			tempSetting = translateEncoderData(skipSetting, 100, 0);

			if(tempSetting != previousSkipSetting)
			{

				previousSkipSetting = tempSetting;
			    char result[50];
			    sprintf(result, "%3d", tempSetting);
				ssd1306_Fill(Black);
			    ssd1306_SetCursor(0,25);
			    ssd1306_WriteString("SKP", Font_16x26, White);
			    ssd1306_WriteString(result, Font_16x26, White);
			    ssd1306_WriteString("%", Font_16x26, White);
				wasDisplayDataChanged = 1;
				if(tempSetting != skipSetting)
				{
					skipSetting = tempSetting;
					previousSkipSetting = tempSetting;
					mainTimer = solarTimeLimit * skipSetting/100.0;
				}

				encoderPoint = getEncoderChange();
			}

		break;
		case MISC:
			if(wasStateChanged)
			{
				encoderPoint = getEncoderChange();
				wasStateChanged = 0;
				miscIterator = 0;
				wasDisplayDataChanged = 1;
			}

			if (hasButtonBeenReleased())
			{
				if (miscIterator == 0)	changeState(SETTINGS);
				else miscVal[miscIterator] = !miscVal[miscIterator];
				wasDisplayDataChanged = 1;

			}
			miscIterator = translateEncoderData(miscIterator, 7, 0);
			ssd1306_Fill(Black);
		    if(miscIterator) ssd1306_SetCursor(0,25);
		    else ssd1306_SetCursor(30,25);
		    ssd1306_WriteString(miscNames[miscIterator], Font_16x26, White);
		    if (miscIterator != 0 and miscVal[miscIterator] == 0) ssd1306_WriteString(" [ ]", Font_16x26, White);
		    else if(miscIterator) ssd1306_WriteString(" [X]", Font_16x26, White);
		    miscFlicker = miscVal[1];
		    miscEternal = miscVal[2];
		    miscDirection = miscVal[3];;
		    miscPause = miscVal[4];
		    miscRepeat = miscVal[5];
		    miscPicMode = miscVal[6];
		    miscScreenOff = miscVal[7];
		    if(currentTimeState != EXPLOSION) flickerEnable = miscFlicker;
		break;
	}
	//if (wasEncoderMoved()) wasDisplayDataChanged = 1;
	if(wasDisplayDataChanged) handleDisplayRefresh();

	currentState = nextState;
}
void timeHandler()
{

	switch(currentTimeState)
		{
			case DISABLED:
				if(wasTimeStateChanged)
				{
					setStepperEnable(GPIO_PIN_SET);
					setRGBoutput(sunOff);
					flickerEnable = 0;
					wasDisplayDataChanged = 1;
					wasTimeStateChanged = 0;
				}
				if(IsDFBusy()) DF_Pause();

				if(wasButtonHeldForPeriod(buttonHeldTime, HAL_GetTick()))
				{
					lastMinute = 999;
					if(miscEternal)	changeTimeState(FROZEN);
					else changeTimeState(NORMAL);

				}

				resetTimer();
				setStepperRate(0);

			break;
			case FROZEN:
				if(wasTimeStateChanged)
				{
					randomizeSongList(HAL_GetTick());
					setupState(GPIO_PIN_RESET, healthySun);
					wasTimeStateChanged = 0;
					flickerEnable = miscFlicker;
					HAL_Delay(20);
					wasDisplayDataChanged = 1;
					setStepperDirection(miscDirection);


				}
				if(wasButtonHeldForPeriod(buttonHeldTime, HAL_GetTick()))
				{
					lastMinute = 999;
					changeTimeState(DISABLED);
				}
				if(!IsDFBusy()) playNextSong();



			break;
			case NORMAL:
				if(wasTimeStateChanged)
				{
					randomizeSongList(HAL_GetTick());
					setupState(GPIO_PIN_RESET, healthySun);
					flickerEnable = miscFlicker;
					wasDisplayDataChanged = 1;
					HAL_Delay(20);
					setStepperDirection(miscDirection);


				}
				if(!IsDFBusy())	playNextSong();

				if(wasButtonHeldForPeriod(buttonHeldTime, HAL_GetTick())) {changeTimeState(DISABLED);

				}

				if(mainTimer >= solarTimeLimit - fadeoutTime) fadeoutAudio(mainTimer, fadeoutTime, solarTimeLimit);
				if (mainTimer <= solarTimeLimit) transitionRGBcolor(healthySun, redSun, solarTimeLimit, mainTimer);
				else changeTimeState(ENDTIMES);


			break;
			case ENDTIMES:
				if(wasTimeStateChanged)
				{
					setupState(GPIO_PIN_RESET, redSun);
					DF_PlaySelected(end_times);
					HAL_Delay(100);
					flickerEnable = miscFlicker;
					wasDisplayDataChanged = 1;
					realTimer = 0;

				}
				if(wasButtonHeldForPeriod(buttonHeldTime, HAL_GetTick())) changeTimeState(DISABLED);

				if(IsDFBusy()) setRGBoutput(redSun);
				else changeTimeState(EXPLOSION);


			break;
			case EXPLOSION:
				if(wasTimeStateChanged)
				{
					setupState(GPIO_PIN_RESET, redSun);
					DF_PlaySelected(supernova);
					HAL_Delay(50);
					wasDisplayDataChanged = 1;
					explosionIterator = 0;
					flickerEnable = 0;
					realTimer = 0;

				}
				if(wasButtonHeldForPeriod(buttonHeldTime, HAL_GetTick())) changeTimeState(DISABLED);

				if(IsDFBusy()) explodeSun();
				else if(miscRepeat) changeTimeState(NORMAL);
				else changeTimeState(DISABLED);

			break;

		}
	currentTimeState = nextTimeState;
}
void resetTimer()
{
	mainTimer = 0;
	realTimer = 0;
}
void updateTime()
{
	uint32_t currentTime = HAL_GetTick();
	if(!miscPause) mainTimer += (currentTime - lastTime)  * timeModifier;
	realTimer += (currentTime - lastTime);
	lastTime = currentTime;
}
void changeState(int state)
{
	wasStateChanged = 1;
	nextState = state;

}
void changeTimeState(int state)
{
	wasTimeStateChanged = 1;
	nextTimeState = state;
}
void setRGBoutput(u8_t data[3])
{
	for(int x = 0; x<3; x++)
	{
		RGBdata[x] = data[x];
	}
}
void transitionRGBcolor(u8_t color1[3], u8_t color2[3], double timeLimit, u32_t timer)
{

	if (timer > timeLimit) timer = timeLimit;
	for(int x = 0; x<3; x++)
	{
		RGBdata[x] = color1[x] - (color1[x] - color2[x])*(timer/timeLimit);
	}
}
std::tuple<int, int> getTime(uint32_t timer)
{
	   int minutes = timer / 60000;
	   int seconds = (timer % 60000) / 1000;
	   return std::make_tuple(minutes, seconds);

}
void setupState(GPIO_PinState motorOn, u8_t sunColor[3])
{
	DF_Pause();
	restoreAudioVolume();
	setStepperEnable(motorOn);
	setStepperRate(speedSetting * 1000/15.0);
	resetTimer();
	updateTime();
	setRGBoutput(sunColor);
	wasTimeStateChanged = 0;

}
void explodeSun()
{
	if(realTimer >= explosionTimes[explosionIterator])
	{
		explosionIterator++;
		if(explosionIterator>6) explosionIterator = 6;
		realTimer = 0;
	}
	transitionRGBcolor(explosionRGB[explosionIterator], explosionRGB[explosionIterator+1], explosionTimes[explosionIterator], realTimer);
}
int translateEncoderData(int data, int maxValue, int MinValue)
{
	data += static_cast<int16_t>(getEncoderChange() - encoderPoint);
	encoderPoint = getEncoderChange();
	if (data < MinValue) data = MinValue;
	if (data > maxValue) data = maxValue;
	return data;

}
void interruptFlicker()
{
	if(HAL_GetTick()-lastTime > 15)
	{
		if(flickerEnable) ARGB_Flicker(HAL_GetTick()-lastTime, RGBdata, 25,  500, lightLevel);
		else ARGB_SetRGB(1,RGBdata[0], RGBdata[1], RGBdata[2]);

		updateTime();

		 while (ARGB_Show() != ARGB_OK);

	}

}
void handleDisplayRefresh()
{
	if ((HAL_GetTick() - refreshTimer) > 50)
	{
		ssd1306_UpdateScreen();
		refreshTimer = HAL_GetTick();
		wasDisplayDataChanged = 0;
	}
}
