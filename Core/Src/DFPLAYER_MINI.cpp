/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */


#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "main.h"
#include "songs.h"
#include <stdlib.h>
extern UART_HandleTypeDef huart2;
#define DF_UART &huart2

#define Source      0x02  // TF CARD

#define Busy_key DFplayer_busy_Pin
#define Busy_port DFplayer_busy_GPIO_Port

int ispause =0;
int isplaying=1;
int mainVolume = 0;
int fadeVolume = 0;
uint8_t songPointer = 2;

# define Start_Byte 0x7E
# define End_Byte   0xEF
# define Version    0xFF
# define Cmd_Len    0x06
# define Feedback   0x00    //If need for Feedback: 0x01,  No Feedback: 0

void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
	uint16_t Checksum = Version + Cmd_Len + cmd + Feedback + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

	uint8_t CmdSequence[10] = { Start_Byte, Version, Cmd_Len, cmd, Feedback, Parameter1, Parameter2, (Checksum>>8)&0x00ff, (Checksum&0x00ff), End_Byte};

	HAL_UART_Transmit(DF_UART, CmdSequence, 10, HAL_MAX_DELAY);
}
void DF_SetVolume(uint8_t volume)
{
	mainVolume = volume;
	fadeVolume = volume;
	Send_cmd(0x06, 0x00, volume);
	HAL_Delay(10);


}
void DF_PlaySelected(uint16_t song)
{
	uint8_t num1 = song>>8;
	uint8_t num2 = song;
  Send_cmd(0x12,num1, num2);
  HAL_Delay(10);
}


void DF_Init (uint8_t volume)
{
	Send_cmd(0x3F, 0x00, Source);
	HAL_Delay(100);
	Send_cmd(0x06, 0x00, volume);
	HAL_Delay(100);
}

void DF_Next (void)
{
	Send_cmd(0x01, 0x00, 0x00);
	HAL_Delay(10);
}

void DF_Pause (void)
{
	Send_cmd(0x0E, 0, 0);
	HAL_Delay(10);
}

void DF_Previous (void)
{
	Send_cmd(0x02, 0, 0);
	HAL_Delay(10);
}

void DF_Playback (void)
{
	Send_cmd(0x0D, 0, 0);
	HAL_Delay(10);
}
int IsDFBusy()
{
	return !(HAL_GPIO_ReadPin(Busy_port, Busy_key));
}
void fadeoutAudio(uint32_t mainTimer, int fadeoutPeriod, double timeLimit)
{
	if(mainTimer >= timeLimit -  fadeVolume*((float)fadeoutPeriod/(float)mainVolume) and fadeVolume)
	{
		if(mainTimer <= timeLimit)
		{
			fadeVolume--;
			Send_cmd(0x06, 0x00, fadeVolume);
			HAL_Delay(5);
		}
		else
		{
			fadeVolume = 0;
			Send_cmd(0x06, 0x00, fadeVolume);
			HAL_Delay(5);

		}

	}

}
void restoreAudioVolume()
{
	DF_SetVolume(mainVolume);
	fadeVolume = mainVolume;

}

void randomizeSongList(uint32_t time)
{
//	for (int i=0; i<songCount; i++)
//	{
//		songList.push_back(i+2);
//	}
//	for(int i=0; i<songCount; i++)
//	{
//		int ranVal = HAL_get % songCount-i +2;
//		songList.push_back(templist.at(ranVal));
//		templist.erase(templist.begin() +ranVal);
//	}
    //std::random_shuffle(songList.begin(),songList.end());
	songPointer = (time % songCount) +2;
}

void playNextSong()
{
	DF_PlaySelected(songPointer);
	songPointer++;
	if(songPointer > songCount +1) songPointer = 2;
	HAL_Delay(50);

}
int getDFVolume()
{
	return mainVolume;
}






