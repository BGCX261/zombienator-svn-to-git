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

#include "guns.h"

//For shooting at zombies
void shoot(ItemStruct shots[2], PlayerStruct* player)
{	
	unsigned char nr;

	if(player->gundelay < 35) //almost optimal shotgun delay!
		return;

	//are any sprite slots free?
	if(shots[0].y >= DISABLED_SPRITE)
		nr = 0;
	else if(shots[1].y >= DISABLED_SPRITE)
		nr = 1;
	else
		return;

	if(shots[nr].y == DISABLED_SPRITE) //if the sprite isn't in use
	{
		shots[nr].y = player->y;
		shots[nr].x = player->x;
		shots[nr].dir = player->sprite.body;
		player->gundelay = 0;
		sprites[shots[nr].sprite.nrsprite].tileIndex = shots[nr].sprite.nranim;

		switch(player->sprite.body) //we want the shot to appear infront of our hero, not on him.
		{
			case 0:
					shots[nr].y+=5;
				break;
			case 1:
					shots[nr].y-=5;
				break;
			case 2:
					shots[nr].x-=3;
				break;
			case 3:
					shots[nr].x+=3;
				break;
		}

		TriggerFx(18,0x90,true); //BOOM CHICK-CHICK
	}

}

//Keeps the shots going until they hit something
void handleshot(MainStruct* mains, GameStruct* game, PlayerStruct* player, ItemStruct *shot, ItemStruct zombies[MAX_ZOMBIES])
{
	postoscroll(shot, mains);
	unsigned char deltax, deltay;
	unsigned int shotkills = 0;
	deltax = mains->x.TILE;
	deltay = mains->y.TILE;

	if(shot->y != DISABLED_SPRITE) //the shot better be onscreen
	{
		for(int i = 0; i<game->zombies; i++)
		{
			if(zombies[i].kill != 0)
				continue;

			//HIT!
			if(abs(shot->y - zombies[i].y) < deltay && abs(shot->x - zombies[i].x) < deltax)
			{
				zombies[i].kill = 1;
				player->killz++;
				player->conskills++;
				shotkills++;

				if(shotkills == 0)
				{
					i = 0;
					deltax += 4;
					deltay += 5;
				}
				else if(shotkills >= 3)
					break;
			}
			
		}

		if(shotkills)
		{
			shot->y = shot->x = DISABLED_SPRITE;
			player->flagcount = 0;
			game->flagcount = 0;

			if(player->conskills >= 8) //INSANE!!!11111
			{
				game->flags |= GAMEMULTIHO|GAMEMULTILO;
				player->killz += 15 * shotkills;
				TriggerFx(22,0x90,false); //BLIDIBLONG
			}
			else if(player->conskills >= 3) //TRIPLE!!
			{
				game->flags |= GAMEMULTIHO;
				game->flags &= ~GAMEMULTILO;
				player->killz += 8 * shotkills;
				TriggerFx(21,0x90,false); //BLODIBLING
			} 
			else if(player->conskills == 2) //DOUBLE!
			{
				game->flags &= ~GAMEMULTIHO;
				game->flags |= GAMEMULTILO;
				player->killz += 4 * shotkills;
				TriggerFx(20,0x90,false); //BLING!
			}
			else //meh.
				TriggerFx(19,0x90,false); //SPLOSCH
			
		}

		switch(shot->dir) //move it.
		{
			case 0:
					shot->y+=5;
				break;
			case 1:
					shot->y-=5;
				break;
			case 2:
					shot->x-=3;
				break;
			case 3:
					shot->x+=3;
				break;
		}

		//disable the sprite if the shot hits a tombstone
		//or if it is offscreen.
		if(shot->y < 0
		|| shot->x < 0
		|| shot->y > SCREEN_HEIGHT
		|| shot->x > SCREEN_WIDTH
		|| (mapcollide(mains, shot->x+1, shot->y+3)
		&&  mapcollide(mains, shot->x+4, shot->y+8)))
			shot->y = shot->x = DISABLED_SPRITE;
		

		drawsprite(shot);	
	}
}