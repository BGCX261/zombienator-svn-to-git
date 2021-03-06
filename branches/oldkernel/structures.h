/*
 *  Zombienator
 *  Copyright (C) 2009 Peter Hedman
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

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <avr/pgmspace.h>
#include "kernel/uzebox.h"

#define MAP_MEGA_WIDTH 12
#define MAP_MEGA_HEIGHT 24
extern const char tiles[];
#define MAINCHAR_SIZE 44
extern const char graveyard_sprites[];
extern const char zombienator_title[];
extern const char graveyard_map[MAP_MEGA_WIDTH][MAP_MEGA_HEIGHT][34];
extern const char intro_map[];
extern const char title_map[];
extern const char fonts[];
extern const char *patches[];

//Should be const in some way, variables for screen pos and scrolling.
//instead of containing both x and y values
//I use two instances of this struct for x and y.
typedef struct CoordStruct
{
	unsigned char s; //screen-scrolling position relative to the vram position
	unsigned char p; //vram position in the megatile map
	unsigned char SCREENTILE; //number of tiles onscreen
	unsigned char TILE; //size of the tile in pixels
	unsigned char MAP; //map size in megatiles
	unsigned char MEGATILE; //megatile size in pixels
	unsigned char MEGA; //megatile size in tiles
} CoordStruct;

//Animation variables, everything animated uses this
typedef struct AnimStruct
{
	unsigned char body;
	unsigned char legs;
	unsigned char speed;
	unsigned char nrsprite; //sprite position in sprite array
	unsigned char nranim; //sprite position in sprite table
	unsigned char animdelay; //to slow down the animation abit
	char animcount; //to keep looping animations in check.
} AnimStruct;

typedef struct PlayerStruct
{
	char life;

	unsigned char x;
	unsigned char y;
	unsigned char flags;
	unsigned char flagcount;
	unsigned char gundelay; //shotgun delay
	unsigned char conskills; //consecutive kills, for scoring. 
	unsigned int hurtcount;
	unsigned int killz; //score
	unsigned int joypad; //input ftw!
	AnimStruct sprite;
} PlayerStruct;

//Contains variables related to the mainchar
typedef struct MainStruct
{
	CoordStruct x;
	CoordStruct y;
} MainStruct;

//General game variables
typedef struct GameStruct
{
	unsigned char flags; //instead of lots of booleans
	unsigned char flagcount; //debounce delay for pausing and toggling the menu
	unsigned char menupos; //cursor position in the menu
	unsigned int zombies; //total number of zombies, dead or undead
	unsigned int frames; //for seeding random number generator + skipping frames with modulus
} GameStruct;

//Zombies and shots, basically anything that's not the main char.
typedef struct ItemStruct
{
	unsigned char dir; //for shots, not zombies
	char kill; //For zombies, not shots
	int x; //Position
	int y;
	int sx; //Scrolling position - basically the total scrolling offset
	int sy; 
	AnimStruct sprite;
} ItemStruct;

struct SpriteStruct sprites[32]; // create the sprites

#endif