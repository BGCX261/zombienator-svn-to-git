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

#include "zombienator.h"


const char singlestr[] PROGMEM = "ONE PLAYER";
const char multistr[] PROGMEM = "TWO PLAYERS";
const char highstr[] PROGMEM = "HIGHSCORE";
const char selectstr[] PROGMEM = "->";

void initzombie(ItemStruct* zombie, MainStruct* mains, char nrsprite)
{
	//Manual insertion of the first zombie
	zombie->x = 15;
	zombie->y = 15;
	zombie->sx = (mains->x.p * mains->x.MEGATILE) + mains->x.s;
	zombie->sy = (mains->y.p * mains->y.MEGATILE) + mains->y.s;
	zombie->kill = 3;

	//Sprites and animations, every zombie will copy this info.
	zombie->sprite.nrsprite = nrsprite;
	zombie->sprite.nranim = MAINCHAR_SIZE + 1;
	zombie->sprite.animcount = 1;
	zombie->sprite.speed = 1;
}

void setoverlay(unsigned char height)
{
	screenSections[1].height = SCREEN_HEIGHT - TILE_HEIGHT * height;
	screenSections[1].vramBaseAdress = vram + VRAM_TILES_H * height;

	screenSections[0].height = TILE_HEIGHT * height;
	screenSections[0].vramBaseAdress = vram;
}

void initgraphics(MainStruct* mains)
{
	SetFontTilesIndex(67);
	InitMusicPlayer(patches);

	//Screen info horziontal
	mains->x.SCREENTILE = SCREEN_TILES_H;
	mains->x.TILE = TILE_WIDTH;
	mains->x.MEGATILE = TILE_WIDTH * MEGATILE_WIDTH;
	mains->x.MAP = MAP_MEGA_WIDTH;
	mains->x.MEGA = MEGATILE_WIDTH;

	//Screen info vertical
	mains->y.SCREENTILE = SCREEN_TILES_V;
	mains->y.TILE = TILE_HEIGHT;
	mains->y.MEGATILE = TILE_HEIGHT * MEGATILE_HEIGHT;
	mains->y.MAP = MAP_MEGA_HEIGHT;
	mains->y.MEGA = MEGATILE_HEIGHT;
	
	//Screen position
	mains->x.p = 2;
	mains->y.p = 6;

	screenSections[1].flags = SCT_PRIORITY_SPR;
	screenSections[1].tileTableAdress = tiles;
	screenSections[1].scrollX = screenSections[1].scrollY = 0;

	screenSections[0].flags = SCT_PRIORITY_BG;
	screenSections[0].tileTableAdress = tiles;


	sprites[0].tileIndex=0;
	SetSpritesOptions(SPR_OVERFLOW_ROTATE);
}

void init(MainStruct *mains, PlayerStruct players[], char nrplayers, GameStruct *game, ItemStruct zombie[MAX_ZOMBIES], ItemStruct shots[][2])
{
	ClearVram();
	srand(game->frames);

	//Set everything to 0
	memset(mains, 0, sizeof(MainStruct));
	memset(players, 0, sizeof(PlayerStruct)*nrplayers);
	memset(game, 0, sizeof(GameStruct));
	memset(zombie, 0, sizeof(ItemStruct)*MAX_ZOMBIES);
	memset(shots, 0, sizeof(ItemStruct)*4);
	memset(sprites, 0, sizeof(struct SpriteStruct)*32);

	//init graphics as well
	initgraphics(mains);
	setoverlay(nrplayers);

	for(int p = 0; p < nrplayers; p++)
	{
		//Main char sprites and guns
		players[p].sprite.nrsprite = 1 + 4 * p;
		players[p].sprite.animcount = 0;
		players[p].sprite.nranim = 1 + 22 * p;

		sprites[players[p].sprite.nrsprite].tileIndex=1 + 22 * p;
		sprites[players[p].sprite.nrsprite+1].tileIndex=11 + 22 * p;
		sprites[players[p].sprite.nrsprite].x = sprites[players[p].sprite.nrsprite+1].x = players[p].x;
		sprites[players[p].sprite.nrsprite].y = players[p].y;
		sprites[players[p].sprite.nrsprite+1].y = players[p].y + TILE_HEIGHT;

		//Init the shots as well.
		for(int i = 0; i < 2; i++)
		{
			shots[p][i].sprite.nrsprite = players[p].sprite.nrsprite + 2 + i;	
			shots[p][i].sprite.nranim = 64;
			shots[p][i].x = shots[p][i].y = DISABLED_SPRITE;
			shots[p][i].kill = NOT_ZOMBIE;
			sprites[shots[p][i].sprite.nrsprite].tileIndex = 64;
		}

		//init player variables
		players[p].x = 72 + 10 * p; //entry point yeah!
		players[p].y = 166;
		players[p].life = 3;
		players[p].gundelay = 35;
	}

	//init various game variables
	game->zombies = 1;
	initzombie(&(zombie[0]), mains, 9);

	return;
}

void gameflags(GameStruct* game)
{
	if(game->flagcount < 235)
		game->flagcount++;

	if(game->flags & GAMEMULTIHO && game->flagcount >= DEBOUNCE_DELAY)
		game->flags &= ~GAMEMULTIHO;

	if(game->flags & GAMEMULTILO && game->flagcount >= DEBOUNCE_DELAY)
		game->flags &= ~GAMEMULTILO;

}

void playerflags(PlayerStruct* player)
{
	if(player->conskills && player->flagcount >= SCORE_DELAY)
		player->conskills = 0;

	if(player->flags & PLAYERHURT && player->hurtcount >= HURT_DELAY)
		player->flags &= ~PLAYERHURT;

	if(player->hurtcount < HURT_DELAY)
		player->hurtcount++;

	if(player->flagcount < 235)
		player->flagcount++;
}

int gameovermenu(GameStruct* game, PlayerStruct* player)
{
	game->flags |= GAMEPAUSED;
	setoverlay(3);

	if(player->life == 0)
	{
		switch(player->joypad&(BTN_SELECT|BTN_START|BTN_UP|BTN_DOWN))
		{
			case BTN_START:
			case BTN_SELECT:
					if(game->menupos == 0)
						return 1;
					else
						return 2;
				break;
			case BTN_UP:
			case BTN_DOWN:
					game->menupos = (game->menupos == 0) ? 1 : 0;
					player->life = -15;
				break;
		}
	}
	else
		player->life++;
	return 0;
}

void inputanim(PlayerStruct* player)
{
	player->sprite.legs = 0;
	player->sprite.speed = 1;

	if(player->joypad&BTN_DOWN)
	{
		player->sprite.body = 0;
		player->sprite.legs = 19;	
	}
	else if(player->joypad&BTN_UP)
	{
		player->sprite.body = 1;
		player->sprite.legs = 16;
	}
	
	if(player->joypad&BTN_LEFT)
	{
		player->sprite.body = 2;
		player->sprite.legs = 4;
	}
	else if(player->joypad&BTN_RIGHT)
	{
		player->sprite.body = 3;
		player->sprite.legs = 10;
	}
	
	switch(player->joypad&(BTN_B|BTN_X|BTN_Y|BTN_A))
	{
		case BTN_B:
				switch(player->sprite.body)
				{
					case 1:
						player->sprite.legs = 19;
						break;
					case 2:
						player->sprite.legs = 7;
						break;
					case 3:
						player->sprite.legs = 13;
						break;
				}
				player->sprite.body = 0;
			break;
		case BTN_X:
				if(player->sprite.body == 0)
					player->sprite.legs = 16;
				player->sprite.body = 1;
			break;
		case BTN_Y:
				if(player->sprite.body == 0)
					player->sprite.legs = 16;
				if(player->sprite.body == 3)
				{
					player->sprite.speed = -1;
					player->sprite.legs = 6;
				}
				player->sprite.body = 2;
			break;
		case BTN_A:
				if(player->sprite.body == 0)
					player->sprite.legs = 16;
				if(player->sprite.body == 2)
				{
					player->sprite.speed = -1;
					player->sprite.legs = 12;
				}
				player->sprite.body = 3;
			break;
	}
	
	animchar(&(player->sprite));
}

void singlemove(PlayerStruct* player, MainStruct* mains, ItemStruct shots[2])
{
	if(player->joypad&BTN_DOWN && !mapcollide(mains, player->x + 3, player->y + 14))
		scrollchar(true, &(mains->y), &(player->y));

	else if(player->joypad&BTN_UP && !mapcollide(mains, player->x + 3, player->y + 11))
		scrollchar(false, &(mains->y), &(player->y));

	if(player->joypad&BTN_LEFT && !mapcollide(mains, player->x + 1, player->y + 13))
		scrollchar(false, &(mains->x), &(player->x));

	else if(player->joypad&BTN_RIGHT && !mapcollide(mains, player->x + 5, player->y + 13))
		scrollchar(true, &(mains->x), &(player->x));

	if(player->joypad&BTN_SR)
		shoot(shots, player);
}

void multimove(PlayerStruct players[2], MainStruct* mains,ItemStruct shots[2][2])
{
	unsigned char canscroll[2];
	memset(&canscroll, 0, sizeof(char) * 2);

	for(int p = 0; p < 2; p++)
	{
		if(players[p].x <= (mains->x.TILE<<3))
			canscroll[1 - p] |= LEFTBORDER;
		else if(players[p].x >= (mains->x.SCREENTILE - 6) * mains->x.TILE)
			canscroll[1 - p] |= RIGHTBORDER;

		if(players[p].y <= (mains->y.TILE<<3))
			canscroll[1 - p] |= TOPBORDER;
		else if(players[p].y >=(mains->y.SCREENTILE - 6) * mains->y.TILE) 
			canscroll[1 - p] |= BOTTOMBORDER;
	}

	for(int p = 0; p < 2; p++)
	{
		if(players[p].joypad&BTN_DOWN && !mapcollide(mains, players[p].x + 3, players[p].y + 14))
		{
			if(!(canscroll[p]&TOPBORDER) && canscroll[1 - p]&BOTTOMBORDER)
			{
				if(scrollchar(true, &(mains->y), &(players[p].y)))
				players[1 - p].y--;
			}
			else
				movechar(true, &(players[p].y), &(mains->y));
		}

		if(players[p].joypad&BTN_UP && !mapcollide(mains, players[p].x + 3, players[p].y + 11))
		{
			if(!(canscroll[p]&BOTTOMBORDER) && canscroll[1 - p]&TOPBORDER)
			{
				if(scrollchar(false, &(mains->y), &(players[p].y)))
				players[1 - p].y++;
			}
			else
				movechar(false, &(players[p].y), &(mains->y));
		}

		if(players[p].joypad&BTN_LEFT && !mapcollide(mains, players[p].x + 1, players[p].y + 13))
		{
			if(!(canscroll[p]&RIGHTBORDER) && canscroll[1 - p]&LEFTBORDER)
			{
				if(scrollchar(false, &(mains->x), &(players[p].x)))
				players[1 - p].x++;
			}
			else
				movechar(false, &(players[p].x), &(mains->x));
		}

		if(players[p].joypad&BTN_RIGHT && !mapcollide(mains, players[p].x + 5, players[p].y + 13))
		{
			if(!(canscroll[p]&LEFTBORDER) && canscroll[1 - p]&RIGHTBORDER)
			{
				if(scrollchar(true, &(mains->x), &(players[p].x)))
				players[1 - p].x--;
			}
			else
				movechar(true, &(players[p].x), &(mains->x));
		}

		if(players[p].joypad&BTN_SR)
			shoot(shots[p], &(players[p]));
	}
}


void rendermap(MainStruct* mains, unsigned char nrplayers)
{
	for(int i = mains->x.p; i < mains->x.p+4; i++)
	{
		for(int u = mains->y.p; u < mains->y.p+8; u++)
		  DrawMap2(((i-mains->x.p)<<3),((u-mains->y.p)<<2) + nrplayers,graveyard_map[i][u]);
	}
}

char singleloop(GameStruct* game)
{
	PlayerStruct singlep;
	MainStruct mains;
	ItemStruct zombie[MAX_ZOMBIES];
	ItemStruct shots[2];

	init(&mains, &singlep, 1, game, zombie, shots);

	for(;; game->frames++)
	{
		WaitVsync(1);

		drawoverlay(game, &singlep);
		gameflags(game);
		playerflags(&singlep);

		singlep.joypad = ReadJoypad(0);

		if(singlep.life <= 0)
		{
			switch(gameovermenu(game, &singlep))
			{
				case 1:
					return 1;
				case 2:
					return 0;
				default:
					break;
			}
		}
		else if(singlep.joypad&BTN_SELECT && game->flagcount >= DEBOUNCE_DELAY)
		{
			game->flagcount = 0;
			if(game->flags & GAMEPAUSED)
				game->flags &= ~GAMEPAUSED;
			else
				game->flags |= GAMEPAUSED;
		}
	
		if(game->flags & GAMEPAUSED)
			continue;
		
		rendermap(&mains, 1);

		screenSections[1].scrollX = mains.x.s;	
		screenSections[1].scrollY = mains.y.s;

		singlemove(&singlep, &mains, shots);
		inputanim(&singlep);

		singlep.gundelay++;
		for(int i = 0; i < 2; i++)
			handleshot(&mains, game, &singlep, &shots[i], zombie);
 
		
		necromancer(zombie, &mains, game, &singlep, 1);

		if(!(singlep.flags & PLAYERHURT) || game->frames&2)
		{
			sprites[1].x = sprites[2].x = singlep.x;
			sprites[1].y = singlep.y;
			sprites[2].y = singlep.y + TILE_HEIGHT;
		}
		else
		{
			sprites[1].x = sprites[2].x = DISABLED_SPRITE;
			sprites[2].y = sprites[1].y = DISABLED_SPRITE;
		}

	}

}

char multiloop(GameStruct* game)
{
	PlayerStruct players[2];
	MainStruct mains;
	ItemStruct zombie[MAX_ZOMBIES];
	ItemStruct shots[2][2];

	init(&mains, players, 2, game, zombie, shots);

	for(;;game->frames++)
	{
		WaitVsync(1);

		multidrawoverlay(game, players);
		gameflags(game);

		for(int p = 0; p < 2; p++)
		{
			playerflags(&players[p]);
			players[p].joypad = ReadJoypad(p);
		}

		if(players[0].life <= 0 && players[1].life <= 0)
		{
			switch(gameovermenu(game, &players[0]))
			{
				case 1:
					return 2;
				case 2:
					return 0;
				default:
					break;
			}
		}
		else if((players[0].joypad&BTN_SELECT || players[1].joypad&BTN_SELECT) && game->flagcount >= DEBOUNCE_DELAY)
		{
			game->flagcount = 0;
			if(game->flags & GAMEPAUSED)
				game->flags &= ~GAMEPAUSED;
			else
				game->flags |= GAMEPAUSED;
		}
	
		if(game->flags & GAMEPAUSED)
			continue;

		rendermap(&mains, 2);
		screenSections[1].scrollX = mains.x.s;
		screenSections[1].scrollY = mains.y.s;

		if(players[0].life > 0 && players[1].life > 0)
			multimove(players, &mains, shots);
		else if(players[0].life <= 0)
			singlemove(&(players[1]), &mains, shots[1]);
		else if(players[1].life <= 0)
			singlemove(&(players[0]), &mains, shots[0]);

		necromancer(zombie, &mains, game, players, 2);

		for(int p = 0; p < 2; p++)
		{
			inputanim(&(players[p]));
			players[p].gundelay++; 
			for(int i = 0; i < 2; i++)
				handleshot(&mains, game, &players[p], &shots[p][i], zombie);

			if(!(players[p].flags & PLAYERHURT) || game->frames&2)
			{
				sprites[1 + p * 4].x = sprites[2  + p * 4].x = players[p].x;
				sprites[1  + p * 4].y = players[p].y;
				sprites[2  + p * 4].y = players[p].y + TILE_HEIGHT;
			}
			else
			{
				sprites[1  + p * 4].x = sprites[2  + p * 4].x = DISABLED_SPRITE;
				sprites[2  + p * 4].y = sprites[1  + p * 4].y = DISABLED_SPRITE;
			}
		}


	
	}
}

char introscreen(GameStruct* game)
{
	unsigned char counter;
	unsigned int joypad;

	ClearVram();
	memset(sprites, 0, sizeof(struct SpriteStruct)*32);

	SetFontTilesIndex(49);
	screenSections[1].flags = SCT_PRIORITY_BG;
	screenSections[0].tileTableAdress = zombienator_title;
	screenSections[1].scrollX = screenSections[1].scrollY = 0;
	setoverlay(0);

	game->menupos = counter = 0;

	DrawMap2(0,6, title_map);
	for(int i = 0; i < 22; i++)
		SetTile(i, 10, 27 + i);

	for(;;)
	{
		WaitVsync(1);
		Fill(0,13, SCREEN_TILES_H, SCREEN_TILES_V - 13, 0);

		Print(5, 14, singlestr);
		Print(5, 15, multistr);
		Print(5, 16, highstr);
		Print(3, 14 + game->menupos, selectstr);
		joypad = 0;
		counter++;
		game->frames++;
		
		if(counter >= 15)
		{
			counter = 0;
			while(!(joypad&(BTN_START|BTN_SELECT|BTN_UP|BTN_DOWN)))
			{
				joypad = ReadJoypad(0);
				switch(joypad&(BTN_START|BTN_SELECT|BTN_UP|BTN_DOWN))
				{
					case BTN_START:
					case BTN_SELECT:
						if(game->menupos == 0)
							return 1;
						else if(game->menupos == 1)
							return 2;
						break;
					case BTN_UP:
						if(game->menupos > 0 )
							game->menupos--;
						else
							game->menupos = 2;
						break;
					case BTN_DOWN:
						if(game->menupos < 2)
							game->menupos++;
						else
							game->menupos = 0;
						break;
				}
			}
		}
	}


	return 0;
}


int main()
{
	char gametype;
	GameStruct game;

	SetSpritesTileTable(graveyard_sprites);
	gametype = 0;
	memset(&game, 0, sizeof(GameStruct));

	for(;;)
	{
		switch(gametype)
		{
			case 0:
					gametype = introscreen(&game);
				break;
			case 1:
					gametype = singleloop(&game);
				break;
			case 2:
					gametype = multiloop(&game);
				break;
		}

	}

	
} 
