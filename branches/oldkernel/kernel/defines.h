/*
 *  Uzebox kernel build options
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

/** 
 * ==============================================================================
 *
 * This file contains default compilation setting that sets the video mode and 
 * enables certains options. Note that these options affects the
 * memory consomption (RAM & FLASH).
 *
 * Since this is a defaults file that is part of the kernel and affect 
 * all games, it is preferable you define custom compilation options. 
 * In AvrStudio go in: Project->Congiguration Options->Custom Options
 * and add -D switches. 
 * 
 * I.e.: To define video mode 2 add a -DVIDEO_MODE=2 switch under [All files]
 *
 * ===============================================================================
 */

 	/*
	 * Defines the video mode to use. 
	 * For all modes, tiles are 6x8 pixels (horizontal x vertical)
	 *
	 * 0 = Reserved
	 * 1 = 40x28 Tile-only.
	 * 2 = 22x26 Tiles, Sprites and full-screen scrolling.
	 * 
	 */
	#ifndef VIDEO_MODE 
		#define VIDEO_MODE 1
	#endif

	/*
	 * Enable horizontal scrolling for video mode 2.
	 * 
	 * Note: This option needs 9K of flash due to unrolled loops.
	 * 
	 * 0 = no
	 * 1 = yes
	 */	
	#ifndef MODE2_HORIZONTAL_SCROLLING
		#define MODE2_HORIZONTAL_SCROLLING 1
	#endif

	/*
	 * Specify the memory to allocate for the overlay(height in tiles).
	 * The overlay is a section at the top of  the screen that is fixed
	 * and does not scroll. It is used for titles and scores.
	 *
	 * The actual height is controlled programatically.Specifying a 
	 * height of zero disables the overlay. 
	 *
	 * Applies only to video mode 2. 
	 * 
	 */	
	#ifndef	OVERLAY_HEIGHT
		#define OVERLAY_HEIGHT 3
	#endif

	/*
	 * Display the Uzebox logo when the console is reset
	 * 0 = no
	 * 1 = yes
	 */
	#ifndef INTRO_LOGO
		#define INTRO_LOGO 1
	#endif

	/*
	 * Joystick type used on the board.
	 * Note: Will be read from EEPROM in a future release. 
	 *
	 * 0 = SNES
	 * 1 = NES
	 */
	#ifndef JOYSTICK
		#define JOYSTICK 0
	#endif

	/*
	 * Activates the MIDI-IN support. 
	 * Not supported with video mode 2.
	 *
	 * 0 = no
	 * 1 = yes
	 */
	#ifndef MIDI_IN
		#define MIDI_IN 0
	#endif

	/*
	 * Screen center adjustment for mode 1 only.
	 * Useful if your game field absolutely needs a non-even width.
	 * Do not go more than +15/-15.
	 *
	 * Center = 0
	 */
	#ifndef CENTER_ADJUSTMENT
		#define CENTER_ADJUSTMENT 0
	#endif


/*
 * Kernel defines
 */
#ifndef __DEFINES_H_
	#define __DEFINES_H_

	#define DISABLED 0
	#define ENABLED  1

	#define TYPE_SNES 0
	#define TYPE_NES 1

	/*
	 * Kernel settings, do not modify
	 */
	#if VIDEO_MODE == 1
		#define TILE_HEIGHT 8
		#define TILE_WIDTH 6
		
		#define VRAM_TILES_H 40 
		#define VRAM_TILES_V 28
		#define SCREEN_TILES_H 40
		#define SCREEN_TILES_V 28
		
		#define FIRST_RENDER_LINE 20
		#define VRAM_SIZE VRAM_TILES_H*VRAM_TILES_V*2
		#define VRAM_ADDR_SIZE 2 //in bytes

	#elif VIDEO_MODE == 2
		#define TILE_HEIGHT 8
		#define TILE_WIDTH 6

		#define VRAM_TILES_H  32
		#define VRAM_TILES_V 32
		#define SCREEN_TILES_H 22
		#define SCREEN_TILES_V 26
		#define FIRST_RENDER_LINE 24
		#define MAX_SPRITES 32
		#define MAX_SPRITES_PER_LINE 4
		#define SPRITE_STRUCT_SIZE 3
		#define TRANSLUCENT_COLOR 0xfe	
		#define VRAM_SIZE 32*32 //VRAM_TILES_H*VRAM_TILES_V	
		#define OVERLAY_ROW VRAM_TILES_V
		#define X_SCROLL_WRAP VRAM_TILES_H*TILE_WIDTH

		#define VRAM_ADDR_SIZE 1 //in bytes

	#endif


#endif
