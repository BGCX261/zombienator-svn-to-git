/*
 *  Uzebox Kernel functions
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

#ifndef __UZEBOX_H_
#define __UZEBOX_H_

	#include <stdbool.h>
	#include "defines.h"

	/*
	 * Joystick constants & functions
	 */
	#define BTN_SR	   1
	#define BTN_SL	   2
	#define BTN_X	   4
	#define BTN_A	   8
	#define BTN_RIGHT  16
	#define BTN_LEFT   32
	#define BTN_DOWN   64
	#define BTN_UP     128
	#define BTN_START  256
	#define BTN_SELECT 512
	#define BTN_Y      1024 
	#define BTN_B      2048 

	extern unsigned int ReadJoypad(unsigned char joypadNo);


	/*
	 * Video Engine defines & functions
	 */


	#define OVERLAY_PRI_HI  0x80 //the overlay will be over the sprites
	#define OVERLAY_PRI_LO  0x00 //the sprites will appear over the overlay

	#if VRAM_ADDR_SIZE == 1
		extern char vram[];  
	#else
		extern int vram[]; 
	#endif
	
	struct SpriteStruct
	{
		unsigned char x;
		unsigned char y;
		unsigned char tileIndex;
		//const char *addr;
	};

	extern struct SpriteStruct sprites[32];

	extern void SoftReset(void);
	
	extern void SetOverlay(unsigned char params);
	extern void SetScrolling(unsigned char x,unsigned char y);
	extern void ClearVram(void);
	extern void SetTile(char x,char y, unsigned int tileId);
	extern void SetFont(char x,char y, unsigned char tileId);
	extern void RestoreTile(char x,char y);
	extern void LoadMap(void);
	extern void SetFontTilesIndex(unsigned char index);
	extern void SetFontTable(const char *data);
	extern void SetTileTable(const char *data);
	extern void SetSpritesTileTable(const char *data);
	extern void DrawMap(unsigned char x,unsigned char y,const int *map);
	extern void DrawMap2(unsigned char x,unsigned char y,const char *map);

	#if VRAM_ADDR_SIZE == 1
		extern void SetTileMap(const char *data);
	#else
		extern void SetTileMap(const int *data);
	#endif

	extern void Print(int x,int y,const char *string);
	extern void PrintBinaryByte(char x,char y,unsigned char byte);
	extern void PrintHexByte(char x,char y,unsigned char byte);
	extern void PrintHexInt(char x,char y,int byte);
	extern void PrintLong(int x,int y, unsigned long val);
	extern void PrintByte(int x,int y, unsigned char val);
	extern void PrintChar(int x,int y,char c);
	extern void PrintInt(int x,int y, unsigned int,bool zeropad);

	extern unsigned char GetVsyncFlag(void);
	extern void ClearVsyncFlag(void);
	extern void WaitVsync(int count);

	extern void Fill(int x,int y,int width,int height,int tile);
	extern void FontFill(int x,int y,int width,int height,int tile);


	/*
	 * Sound Engine defines & functions
	 */
	 
	//Patch commands
	#define PC_ENV_SPEED	0
	#define PC_NOISE_PARAMS	1
	#define PC_WAVE			2
	#define PC_NOTE_UP		3
	#define PC_NOTE_DOWN	4
	#define PC_NOTE_CUT		5
	#define PC_NOTE_HOLD 	6
	#define PC_ENV_VOL		7
	#define PC_PITCH		8
	#define PC_TREMOLO_LEVEL	9
	#define PC_TREMOLO_RATE	10
	#define PATCH_END		0xff

	/**
	 * Invokes directly the mixer with the specified parameters, bypassing the main engine.
	 * channel=0-3
	 * volume=0-255
	 * pitch=step value
	 * param=wave No (0-9) for chan 0-2, noise parameters for chan 3
	 */
	extern void SetMixerVoice(unsigned char channel,unsigned char volume, unsigned int pitch, unsigned char param);
	
	extern void TriggerNote(unsigned char channel,unsigned char patch,unsigned char note,unsigned char volume);
	extern void TriggerFx(unsigned char patch,unsigned char volume, bool retrig);
	extern void ProcessMusic(void);
	extern void StopSong();
	extern void StartSong(const char *midiSong);
	extern void ResumeSong();
	extern void InitMusicPlayer(const char *patchPointers[]);

	//Sound channels modifier
	extern void PatchCommandSetTremoloLevel(unsigned char channel, char param);
	extern void PatchCommandSetTremoloRate(unsigned char channel, char param);


	/*
	 * Other functions
	 */
	extern void SetLowSpeed(void);
	extern void SetFullSpeed(void);


#endif
