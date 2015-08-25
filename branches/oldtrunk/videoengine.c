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

#include "videoengine.h"

const char scorestr[] PROGMEM = "SCORE:";
const char pausestr[] PROGMEM = "PAUSED";
const char doublestr[] PROGMEM = "DOUBLE!";
const char triplestr[] PROGMEM = "MULTI!";
const char insanestr[] PROGMEM = "INSANE!";
const char overstr[] PROGMEM = "GAME OVER";
const char suboverstr[] PROGMEM =  "RETRY?";
const char suboverstr2[] PROGMEM = "MENU?";
const char choicestr[] PROGMEM = ">";


//All animations use this one.
void animchar(AnimStruct* anim)
{
	if(anim->legs == 0) //no movement.
	{
		if(anim->animcount != 0)
		{
			//revert the animations to the standing position.
			sprites[anim->nrsprite+1].tileIndex -= (anim->animcount - anim->speed) * anim->speed;
			anim->animcount = 0;
		}
		sprites[anim->nrsprite].tileIndex=anim->nranim + anim->body;
		return;
	}
	else if(anim->animdelay >= 6) //slowing down the animation
	{
		anim->animdelay = 0;
		anim->animcount = (anim->animcount < 2 && anim->animcount > -2) ? anim->animcount + anim->speed: 0;
	} 
	else
		anim->animdelay++;

	sprites[anim->nrsprite].tileIndex=anim->nranim + anim->body; //upper body
	sprites[anim->nrsprite+1].tileIndex=anim->nranim + anim->legs + anim->animcount; //lower body
}

//Overlay, what can I say? Score, ammunition, health left, etc.
void drawoverlay(GameStruct* game, PlayerStruct* player)
{
  	Fill(0, OVERLAYROW, 24, 1, 16); //black

	//score
	Print(11, OVERLAYROW, scorestr); 
	PrintInt(21, OVERLAYROW, player->killz, true);

	if(player->life <= 0)
    {
		Fill(0, OVERLAYROW + 1, 24, 2, 16); //black
		Print(0, OVERLAYROW, overstr); //game over
		Print(5, OVERLAYROW + 1, suboverstr); //retry?
		Print(5, OVERLAYROW + 2, suboverstr2); //menu?
		Print(4, OVERLAYROW + 1 + game->menupos, choicestr);
	}
	else
	{
		if(game->flags & GAMEPAUSED)
			Print(4, OVERLAYROW, pausestr); //paused
		else if(game->flags & (GAMEMULTIHO|GAMEMULTILO) && game->frames&2)
		{
			switch(game->flags&(GAMEMULTIHO|GAMEMULTILO))
			{
				case GAMEMULTILO:
						Print(3, OVERLAYROW, doublestr); //DOUBLE
					break;
				case GAMEMULTIHO:
						Print(3, OVERLAYROW, triplestr); //TRIPLE
					break;
				case GAMEMULTIHO|GAMEMULTILO:
						Print(3, OVERLAYROW, insanestr); //INSANE!!
					break;
			}
		}

		//lives left.
		for(int i = 0; i < player->life; i++)
		SetTile(i, OVERLAYROW, 17);
		}
}

void multidrawoverlay(GameStruct* game, PlayerStruct players[2])
{
  Fill(0, OVERLAYROW, 24, 2, 16); //black

	for(int p  = 0; p < 2; p++)	//score
	{
		Print(11 * p, OVERLAYROW, scorestr); 
		PrintInt(10 + 11 * p, OVERLAYROW, players[p].killz, true);
	}

	if(players[0].life <= 0 && players[1].life <= 0)
	{
		Fill(0, OVERLAYROW + 1, 24, 2, 16); //black
		Print(8, OVERLAYROW + 2, overstr); //game over
		Print(1, OVERLAYROW + 1, suboverstr); //retry?
		Print(1, OVERLAYROW + 2, suboverstr2); //menu?
		Print(0, OVERLAYROW + 1 + game->menupos, choicestr);
	}
	else
	{
		for(int p = 0; p < 2; p++)
		{
			if(game->flags & GAMEPAUSED)
				Print(4, OVERLAYROW + 1, pausestr); //paused
			else if(game->flags & (GAMEMULTIHO|GAMEMULTILO) && game->frames&2)
			{
				switch(game->flags&(GAMEMULTIHO|GAMEMULTILO))
				{
					case GAMEMULTILO:
							Print(3, OVERLAYROW+1, doublestr); //DOUBLE
						break;
					case GAMEMULTIHO:
							Print(3, OVERLAYROW+1, triplestr); //TRIPLE
						break;
					case GAMEMULTIHO|GAMEMULTILO:
							Print(3, OVERLAYROW+1, insanestr); //INSANE!!
						break;
				}
			}

			//lives left.
			for(int i = 0; i < players[p].life; i++)
			SetTile(i + 12 * p, OVERLAYROW + 1, 17);
		}
		}
}

//Early attempt at making zombies explode into blood and gore and guts when shot at.
void gorify(ItemStruct* zombie)
{
	//have we had 3 frames of splatter?
	if(zombie->kill >= 3)
	{
		sprites[zombie->sprite.nrsprite].x = sprites[zombie->sprite.nrsprite+1].x = DISABLED_SPRITE;
		sprites[zombie->sprite.nrsprite].y = sprites[zombie->sprite.nrsprite+1].y = DISABLED_SPRITE;
		return;
	}
	//is the slowdown timer complete?
	else if(zombie->sprite.animdelay >= 6)
	{
		zombie->sprite.animdelay = 0;
		zombie->kill++;
	} 
	else
		zombie->sprite.animdelay++;

	sprites[zombie->sprite.nrsprite].tileIndex=zombie->sprite.nranim + 12 + zombie->kill;
	sprites[zombie->sprite.nrsprite+1].tileIndex=zombie->sprite.nranim + 15 + zombie->kill;
}

//Sprite drawing function, handles shots and zombies
void drawsprite(ItemStruct* item)
{
	unsigned char tempx, tempy;

	//the sprite positions are unsigned chars, we don't want over/underflow
	if(( 255 >= item->x ) && ( item->x >= 0 ) && ( 255 >= item->y ) && ( item->x >= 0 ))
	{
		tempx = item->x;
		tempy = item->y;
	}
	else
	{
		tempx = DISABLED_SPRITE;
		tempy = DISABLED_SPRITE;
	}
	
	//quite a hackjob, NOT_ZOMBIE basically means shotgun shot.
	//if kill < 0 then the zombie is rising from his grave 
	if(item->kill != NOT_ZOMBIE && item->kill >= 0)
	{
		//show lower body aswell
		sprites[item->sprite.nrsprite+1].y = tempy + TILE_HEIGHT;
		sprites[item->sprite.nrsprite+1].x = tempx;
	}
	
	sprites[item->sprite.nrsprite].x = tempx;
	sprites[item->sprite.nrsprite].y = tempy;
	
}
