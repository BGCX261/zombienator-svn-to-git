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

#ifndef VIDEOENGINE_H_
#define VIDEOENGINE_H_

#include "zombienator.h"

//All animations use this one.
void animchar(AnimStruct* anim);

//Overlay, what can I say? Score, ammunition, health left, etc.
void drawoverlay(GameStruct* game, PlayerStruct* player);
void multidrawoverlay(GameStruct* game, PlayerStruct players[2]);

//Early attempt at making zombies explode into blood and gore and guts when shot at.
void gorify(ItemStruct* zombie);

//Sprite drawing function, handles shots and zombies
void drawsprite(ItemStruct* item);

#endif