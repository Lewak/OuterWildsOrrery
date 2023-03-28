/*
 * DFPLAYER_MINI.h
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#ifndef INC_DFPLAYER_MINI_H_
#define INC_DFPLAYER_MINI_H_


void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2);
void DF_PlaySelected(uint16_t song);
void DF_SetVolume(uint8_t volume);
void DF_Init (uint8_t volume);
void DF_Next (void);
void DF_Pause (void);
void DF_Previous (void);
void DF_Playback (void);
int IsDFBusy();
void restoreAudioVolume();
void fadeoutAudio(uint32_t deltaTime, int fadeoutPeriod, double timeLimit);
void randomizeSongList(uint32_t time);
void playNextSong();
int getDFVolume();
#endif /* INC_DFPLAYER_MINI_H_ */
