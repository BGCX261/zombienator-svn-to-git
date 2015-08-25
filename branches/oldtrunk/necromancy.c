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

#include "necromancy.h"

//All mighty zombie-super-AI-function
//returns true if the zombie found some tasty hero-brains
char handlezombie(ItemStruct* zombie, PlayerStruct players[], char nrplayers, MainStruct* main)
{
	int deltax[2];
	int deltay[2];
	memset(deltax, 0, sizeof(int)*2);
	memset(deltay, 0, sizeof(int)*2);
	unsigned char target, retvalue;

	//ouch a shotgun wound in my head!
	if(zombie->kill > 0)
	{
		gorify(zombie);	
		return false;
	}
	
	//where are the closest tasty brains?
	for(int p = 0; p < nrplayers; p++)
	{
		deltax[p] = zombie->x - players[p].x; 
		deltay[p] = zombie->y - players[p].y;
	}

	if(abs(deltax[0]) + abs(deltay[0]) < abs(deltax[1]) + abs(deltay[1]) || nrplayers == 1)
		target = 0;
	else
		target = 1;

	zombie->sprite.body = 0;
	zombie->sprite.legs = 0; 

	if(zombie->kill < 0) //if the zombie is rising from his grave
	{
		zombie->sprite.body = 0;
		zombie->sprite.legs = 10;
		zombie->kill++;
	}
	else if(abs(deltax[target]) > abs(deltay[target]))
	{
		if(deltax[target] > 0)
		{
			if(!mapcollide(main, zombie->x + 1, zombie->y + 13))
			zombie->x--;
			else if(deltay[target] >= 0)
			zombie->y--;
			else
			zombie->y++;

			zombie->sprite.body = 2;
			zombie->sprite.legs = 4;
		}
		else if(deltax[target] < 0)
		{
			if(!mapcollide(main, zombie->x + 5, zombie->y + 13))
			zombie->x++;
			else if(deltay[target] >= 0)
			zombie->y--;
			else
			zombie->y++;

			zombie->sprite.body = 3;
			zombie->sprite.legs = 7;
		}
	}
	else
	{
		if(deltay[target] > 0)
		{
			if(!mapcollide(main, zombie->x + 3, zombie->y + 11))
			zombie->y--;
			else if(deltax[target] >= 0)
			zombie->x--;
			else
			zombie->x++;

			zombie->sprite.body = 0;
			zombie->sprite.legs = 10;
		}
		else if(deltay[target] < 0)
		{
			if(!mapcollide(main, zombie->x + 3, zombie->y + 14))
			zombie->y++;
			else if(deltax[target] >= 0)
			zombie->x--;
			else
			zombie->x++;

			zombie->sprite.body = 1;
			zombie->sprite.legs = 10;
		}
	}

	//keep the animations constant when going diagonally
	if(abs(abs(deltax[target])-abs(deltay[target])) < 2) 
	{
		if(deltax[target] > 0)
			zombie->sprite.legs = 4;
		else if(deltax[target] < 0)
			zombie->sprite.legs = 7;

		if(deltay[target] > 0)
			zombie->sprite.body = 0;
		else if(deltay[target] < 0)
			zombie->sprite.body = 1;
	}
	
	animchar(&zombie->sprite);

	//are we close enough for tasty brains?
	retvalue = 0;
	if(abs(deltax[0]) < 6 && abs(deltay[0]) < 7)
		retvalue |= 1;
	if(abs(deltax[1]) < 6 && abs(deltay[1]) < 7)
		retvalue |= 2;

	return retvalue;
}

//ABRA-KADABRA AND THERE WERE ZOMBIES
void summon(ItemStruct* zombie, MainStruct* mains)
{
	unsigned int spawntile = findgrave(mains); //returns random tombstone tile.
	
	//calculate x and y position
	zombie->x = (((spawntile % 32) + 1) * TILE_WIDTH) - mains->x.s;
	zombie->y = (((spawntile >> 5) + 1) * TILE_HEIGHT) - mains->y.s;

	//we want to spawn below the tombstone
	while(mapcollide(mains, zombie->x + 5, zombie->y + 13))
		zombie->y+=2;

	zombie->kill = -18;
	zombie->y++;

	//the zombie animation sprites are after the mainchar animations
	zombie->sprite.nranim = MAINCHAR_SIZE + 1;
	zombie->sprite.animcount = 1;
	zombie->sprite.speed = 1;
}


//CAN WE SUMMON UP SOME FORCES OF DARKNESS? MUAHAHAHAHHAHAHA
void ritual(ItemStruct zombie[MAX_ZOMBIES], GameStruct* game, MainStruct *mains)
{
	if(game->zombies < MAX_ZOMBIES) //if we still havent filled the entire zombie array
	{
		summon(&zombie[game->zombies], mains);

		zombie[game->zombies].sprite.nrsprite = zombie[(game->zombies)-1].sprite.nrsprite + 2;
		game->zombies++;
	}
	else //look for dead zombies in the array.
	{
		for(int i = 0; i < game->zombies; i++)
		{
			if(zombie[i].kill >= 3)
			{	
				summon(&zombie[i], mains);
				break;
			}
		}
	}
}

//General zombie management.
void necromancer(ItemStruct zombie[MAX_ZOMBIES], MainStruct *mains, GameStruct *game, PlayerStruct players[], char nrplayers)
{
	unsigned char hit = 0;

	//always update the scrolling positions and draw the zombies
	for(int i = 0; i < game->zombies; i++)
	{
			postoscroll(&zombie[i], mains);

			if(zombie[i].y + mains->y.s < 0
			|| zombie[i].x + mains->x.s < 0
			|| zombie[i].y + mains->y.s > mains->y.TILE<<5
			|| zombie[i].x + mains->x.s > mains->x.TILE<<5)
			zombie[i].kill = 3;

			if(zombie[i].kill >= 3)
				continue;

			drawsprite(&zombie[i]);
	}

	
	if((game->frames % 3) != 0) //don't handle the zombies every frame.
	{
		for(int i = 0; i < game->zombies; i++)
		{	
			hit |= handlezombie(&zombie[i], players, nrplayers, mains);
		}
	}
	else if(rand()%100 > 91) //handle spawning once every three frames
		ritual(zombie, game, mains);

	
	//have we managed to eat some tasty brains?
	for(int p = 0; p < nrplayers; p++)
	{
		if((hit&(p+1)) && players[p].life > 0 && !(players[p].flags & PLAYERHURT))
		{
			players[p].life--;
			players[p].flags |= PLAYERHURT;
			players[p].hurtcount = 0;
			if(players[p].life <= 0) 
			{
				TriggerFx(24,0x90,false);
				players[p].x = players[p].y = DISABLED_SPRITE;
			}
			else
				TriggerFx(23,0x90,false);
		}
	}
}
