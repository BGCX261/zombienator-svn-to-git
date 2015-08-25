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

#include "mechanics.h"

//Horrible attempt att minimizing the code, unreadable.
//Handles vertical and horizontal scrolling
//Oh, and also moving the main char.
bool scrollchar(bool direction, CoordStruct* mstruct, unsigned char* c)
{
	bool retvalue = false;
	if(direction) //are we moving down/right
	{
		//Is the main character close to the screen border?
		if((*c) < (mstruct->SCREENTILE - 8) * mstruct->TILE) 
			(*c)++;   
		//Is there enough space left to scroll?
		else if(mstruct->s < (VIDEO_MEMORY_SIZE*mstruct->TILE - (mstruct->SCREENTILE - 2)*mstruct->TILE))
		{		
			mstruct->s++;
			retvalue = true;
		}
		//Can we change megatile or are we out of map?
		else if(mstruct->p < (mstruct->MAP - VIDEO_MEMORY_SIZE/mstruct->MEGA))
		{
			mstruct->p++;
			mstruct->s -= (VIDEO_MEMORY_SIZE*mstruct->TILE - (mstruct->SCREENTILE*mstruct->TILE + 1));
			retvalue = true;
		}
		//Is the main character at the border?
		else if((*c) < (mstruct->SCREENTILE - 2)*mstruct->TILE)
			(*c)++;
	}
	else //or up/left
	{
		//Is the main character close to the screen border?
		if((*c) > (mstruct->TILE<<3))
			(*c)--;
		//Is there enough space left to scroll?
		else if(mstruct->s > 0)
		{
			mstruct->s--;
			retvalue = true;
		}
		//Can we change megatile or are we out of map?
		else if(mstruct->p > 0)
		{
			mstruct->p--;
			mstruct->s += (VIDEO_MEMORY_SIZE*mstruct->TILE - (mstruct->SCREENTILE*mstruct->TILE + 1));
			retvalue = true;
		}
		//Is the main character at the border?
		else if((*c) > mstruct->TILE)
			(*c)--;
	}
	return retvalue;
}

void movechar(bool direction, unsigned char* c, CoordStruct* mstruct)
{
	if(direction && (*c) < (mstruct->SCREENTILE - 2)*mstruct->TILE)
		(*c)++; 
	else if((*c) > mstruct->TILE)
		(*c)--;
}


//Adjusts coordinates after scrolling has occured
void postoscroll(ItemStruct* item, MainStruct* main) 
{
	item->x -= ((main->x.p * main->x.MEGATILE) + main->x.s - item->sx);
	item->y -= ((main->y.p * main->y.MEGATILE) + main->y.s - item->sy);
	item->sx = (main->x.p * main->x.MEGATILE) + main->x.s;
	item->sy = (main->y.p * main->y.MEGATILE) + main->y.s;
}

//Collision detection against the background image.
//Reads the vram to get the tombstone coordinates.
bool mapcollide(MainStruct* mains, int x, int y)
{
	unsigned char maptile, tile_x, tile_y;

	//convert pixel values to tile values.
	tile_x = (((x + mains->x.s) / mains->x.TILE) - 1);
	tile_y = (((y + mains->y.s) / mains->y.TILE) - 1);

	maptile = vram[tile_y*32 + tile_x]; //I do hope I'm choosing the right tile here.
	
	if(maptile == 3
	|| maptile == 4
	|| maptile == 5
	|| maptile == 6) //tombstone tile numbers.
		return true;
	else
		return false;
}

//zombies cannot spawn everywhere you know.
int findgrave(MainStruct *mains)
{
	unsigned int gravetile;
	gravetile = rand() % 1024; // size of vram when one-dimensional

	while(vram[gravetile] != 3
	&&	vram[gravetile] != 4
	&&	vram[gravetile] != 5
	&&	vram[gravetile] != 6) // not tombstone
	{
		gravetile+=2; // tombstones are always 2 tiles wide. (I think..)
		if(gravetile >= 959) // no overflow plz (1023 - 64 (if the spawnposition happens to be two rows down))
			gravetile = 0;
	}

	return gravetile;
}
