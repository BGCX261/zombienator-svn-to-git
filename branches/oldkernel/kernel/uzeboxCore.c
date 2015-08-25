/*
 *  Uzebox Kernel
 *  Copyright (C) 2008  Alec Bourque
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "defines.h"
#include "uzebox.h"

#if INTRO_LOGO ==1 
	#include "data/uzeboxlogo.pic.inc"
	#include "data/uzeboxlogo.map.inc"

	//Logo "kling" sound
	const char initPatch[] PROGMEM ={	
	0,
	0,PC_WAVE,8,
	0,PC_PITCH,85,
	4,PC_PITCH,90,
	0,PC_ENV_SPEED,-8,   
	0,PC_TREMOLO_LEVEL,0x90,     
	0,PC_TREMOLO_RATE,30, 
	50,PC_NOTE_CUT,0,
	0,PATCH_END  
	};
	const char *initPatches[] PROGMEM = {initPatch};
#endif 

#if VIDEO_MODE == 2
	#define LOGO_X_POS 8
#else
	#define LOGO_X_POS 18
#endif

extern volatile unsigned int joypad1_status_lo;
extern volatile unsigned int joypad2_status_lo;

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}

/**
 * Performs a software reset
 */
void SoftReset(void){        
	wdt_enable(WDTO_15MS);  
	while(0);
}

/**
 * Read tand retutns the specified joypad buttons.
 * Function also monitors for the reset condition:
 * Select+Start+A+B
 */
unsigned int ReadJoypad(unsigned char joypadNo){
	static unsigned int joy;

	if(joypadNo==0){
		joy=joypad1_status_lo;
	}else{
		joy=joypad2_status_lo;
	}
	
	//process reset (check if joystick is unplugged first)
	if((joy!=0xff) && (joy&BTN_START) && (joy&BTN_SELECT) && (joy&BTN_A) && (joy&BTN_B)){
		while(joypad1_status_lo!=0 && joypad2_status_lo!=0);
		SoftReset();
	}

	return joy;
}

/**
 * Called by the assembler initialization routines, should not be called directly.
 */
void InitConsole(void){

#if INTRO_LOGO ==1 
	InitMusicPlayer(initPatches);	
	SetTileTable(uzeboxlogo);
	SetFontTable(uzeboxlogo);
	
	//draw logo
	ClearVram();
	WaitVsync(15);

	TriggerFx(0,0xff,true);
	DrawMap(LOGO_X_POS,12,map_uzeboxlogo);
	WaitVsync(3);
	DrawMap(LOGO_X_POS,12,map_uzeboxlogo2);
	WaitVsync(2);
	DrawMap(LOGO_X_POS,12,map_uzeboxlogo);
	WaitVsync(40);

	ClearVram();
	WaitVsync(20);
#endif
}

