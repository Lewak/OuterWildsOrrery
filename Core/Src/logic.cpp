#include <bitmap.h>
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
#include "button.h"
#include "encoder.h"
#include "stateMachine.h"

void logic()
{
	  ARGB_Init();
	  ARGB_Clear();
	  while (ARGB_Show() != ARGB_OK);
	  ssd1306_Init();
	  ssd1306_Fill(Black);
	 DF_Init(30);
	 DF_SetVolume(25);
	 setupStepperDriver();

	while (1)
	{
		stateHandler();
	}
}



