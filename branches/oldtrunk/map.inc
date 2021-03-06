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

const char graveyard_map[MAP_MEGA_WIDTH][MAP_MEGA_HEIGHT][34] PROGMEM =
{ 
	
{
{8,4,
0,1,0,1,1,1,1,0,
1,0,0,1,3,4,1,0,
1,0,0,0,6,5,1,0,
0,0,0,0,0,0,1,0},

{8,4,
12,12,11,11,12,11,11,11,
14,14,14,14,14,14,14,14,
1,1,0,0,0,0,0,0,
1,3,4,0,0,0,3,4},

{8,4,
1,6,5,0,0,0,6,5,
1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,
0,3,4,0,0,3,4,1},

{8,4,
0,6,5,0,0,6,5,0,
0,0,0,1,0,0,0,0,
0,0,0,0,0,0,1,1,
1,0,0,0,0,0,1,1},

{8,4,
1,1,0,0,1,0,0,1,
0,3,4,0,0,0,3,4,
0,6,5,0,0,1,6,5,
0,0,1,0,0,1,1,2},

{8,4,
0,1,1,2,2,1,1,2,
0,1,1,2,2,0,2,2,
0,0,0,0,2,1,0,0,
0,3,4,0,1,0,3,4},

{8,4,
0,6,5,0,1,1,6,5,
0,2,0,0,1,1,0,1,
0,0,0,2,1,1,0,2,
0,3,4,0,1,2,3,4},

{8,4,
0,6,5,0,0,0,6,5,
0,0,0,2,1,1,0,0,
1,1,1,0,1,1,0,2,
0,1,1,1,1,1,0,2},

{8,4,
0,0,0,0,2,1,1,1,
2,3,4,0,2,2,0,1,
2,6,5,0,2,2,0,1,
11,11,12,12,12,12,11,11},

{8,4,
14,14,14,14,14,14,14,14,
2,0,0,1,1,1,0,0,
0,0,3,4,0,0,0,3,
0,0,6,5,0,0,0,6},

{8,4,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
11,12,11,12,11,11,11,11,
14,14,14,14,14,14,14,14},

{8,4,
0,0,0,0,0,2,2,2,
0,0,3,4,2,0,3,4,
2,2,6,5,1,1,6,5,
2,1,0,0,0,1,0,1},

{8,4,
1,1,0,1,1,1,1,1,
0,0,1,1,0,0,3,4,
1,1,1,1,0,0,6,5,
0,1,0,0,0,0,0,2},

{8,4,
0,0,1,0,0,0,0,0,
2,2,8,15,11,12,11,12,
2,2,7,15,14,14,14,14,
0,1,7,10,0,0,1,0},

{8,4,
0,0,7,9,0,0,1,0,
0,0,7,10,2,1,1,0,
0,0,8,10,2,1,1,3,
0,0,7,9,2,0,2,6},

{8,4,
0,0,7,10,2,2,0,1,
0,0,7,9,0,0,1,1,
0,0,8,10,0,1,1,1,
0,0,7,9,1,1,0,0},

{8,4,
0,0,7,10,0,0,1,0,
0,0,7,9,0,3,4,0,
0,0,7,10,0,6,5,1,
0,0,8,10,0,1,1,1},

{8,4,
0,0,7,9,1,1,0,1,
1,0,7,15,11,11,11,11,
1,1,7,15,14,15,15,15,
0,0,0,0,0,0,7,9},

{8,4,
1,1,0,3,4,1,7,10,
1,1,1,6,5,0,7,9,
0,0,1,0,0,0,7,10,
1,0,1,3,4,0,8,10},

{8,4,
0,0,0,6,5,0,7,9,
1,0,0,0,0,0,7,10,
0,1,8,15,11,11,15,15,
1,0,7,15,14,14,14,14},

{8,4,
0,0,7,10,1,1,0,0,
1,0,7,9,0,1,1,0,
0,1,7,10,0,3,4,0,
0,0,8,10,1,6,5,0},

{8,4,
0,0,7,9,0,0,0,0,
0,1,7,10,0,0,0,1,
1,1,7,9,0,0,0,1,
0,1,7,10,0,0,0,2},

{8,4,
1,1,8,10,0,0,3,4,
1,0,7,10,0,0,6,5,
1,0,7,9,1,1,0,0,
0,0,7,10,1,0,1,0},

{8,4,
0,0,8,10,1,1,0,0,
0,0,7,9,1,0,1,0,
0,0,7,10,1,3,4,0,
2,0,7,9,0,6,5,0},

},

{

{8,4,
0,0,0,0,0,0,0,2,
0,0,3,4,0,0,0,0,
0,0,6,5,2,0,0,0,
2,0,2,2,2,2,0,2},

{8,4,
11,11,12,12,12,12,11,11,
14,15,15,14,14,14,14,14,
1,7,10,1,0,0,0,2,
1,8,10,0,3,4,0,0},

{8,4,
1,8,10,1,6,5,0,0,
1,8,10,0,1,0,0,2,
1,7,9,0,0,2,0,1,
0,8,10,0,3,4,0,2},

{8,4,
0,8,9,0,6,5,1,0,
0,8,10,0,0,0,2,0,
0,7,15,12,12,11,11,11,
2,7,15,14,14,14,14,14},

{8,4,
1,8,10,1,1,1,1,1,
1,7,10,0,3,4,0,0,
0,8,10,1,6,5,1,0,
1,8,10,0,1,0,1,0},

{8,4,
1,8,10,1,1,1,0,2,
0,7,9,1,1,1,0,2,
1,8,10,1,1,0,0,1,
1,8,9,1,3,4,0,0},

{8,4,
0,8,10,1,6,5,1,2,
1,7,10,0,0,0,0,2,
2,7,10,1,1,0,1,1,
1,8,10,0,3,4,0,0},

{8,4,
0,8,10,0,6,5,1,3,
0,8,10,0,1,1,1,6,
2,7,9,1,1,1,2,1,
2,8,10,0,1,1,2,1},

{8,4,
0,8,9,0,0,0,1,1,
0,8,10,1,3,4,1,1,
0,7,10,0,6,5,1,1,
11,15,15,11,11,12,12,12},

{8,4,
14,15,15,14,14,14,14,14,
0,0,0,0,0,1,1,0,
4,0,0,1,3,4,0,0,
5,0,0,1,6,5,1,1},

{8,4,
0,1,2,2,2,2,2,0,
0,0,0,2,0,0,0,0,
11,12,12,12,12,15,10,0,
14,14,14,14,14,15,10,1},

{8,4,
1,1,0,0,1,8,10,1,
1,0,1,1,0,7,9,0,
1,0,1,1,0,7,10,1,
0,0,0,0,0,8,10,1},

{8,4,
0,0,0,0,1,8,10,1,
0,0,3,4,0,8,10,1,
0,0,6,5,1,7,9,2,
2,0,1,0,2,7,10,1},

{8,4,
2,2,0,2,2,8,10,2,
11,11,11,11,11,15,10,2,
14,14,14,14,14,15,10,2,
0,0,0,2,2,7,15,15},

{8,4,
0,0,2,0,0,7,15,14,
0,1,1,0,1,0,0,0,
4,1,1,0,0,0,1,1,
5,0,0,0,0,0,1,1},

{8,4,
0,1,0,1,0,0,0,0,
1,0,0,3,4,0,0,1,
1,1,0,6,5,0,0,1,
0,1,0,0,0,1,1,1},

{8,4,
0,2,1,1,1,1,1,1,
1,2,1,0,0,3,4,1,
1,2,2,1,1,6,5,0,
1,1,1,2,1,1,0,0},

{8,4,
1,1,0,0,0,2,2,1,
12,12,12,12,11,11,12,11,
15,14,14,14,14,14,14,14,
0,0,1,1,1,1,0,0},

{8,4,
1,3,4,1,1,1,3,4,
1,6,5,1,1,1,6,5,
1,1,1,1,1,1,0,1,
1,1,0,0,3,4,0,1},

{8,4,
0,1,0,0,6,5,0,1,
0,1,1,1,0,0,0,0,
12,12,12,12,11,11,12,11,
14,14,14,14,14,14,14,14},

{8,4,
1,0,0,1,0,0,0,0,
0,0,0,0,0,2,2,1,
1,1,1,0,0,3,4,0,
0,1,2,0,1,6,5,2},

{8,4,
0,2,0,0,0,0,1,0,
1,2,0,0,0,0,1,1,
2,2,1,0,0,1,3,4,
2,0,0,1,0,0,6,5},

{8,4,
0,2,2,0,0,0,0,1,
0,1,2,0,0,0,0,1,
1,1,1,1,0,0,1,0,
0,0,0,3,4,0,1,0},

{8,4,
0,0,1,6,5,0,0,0,
0,0,0,0,0,1,0,0,
0,0,0,0,0,1,1,2,
0,0,0,0,0,1,1,0},

},

{

{8,4,
7,10,0,0,0,2,2,2,
8,10,0,3,4,0,2,2,
8,10,2,6,5,2,0,0,
8,10,0,2,0,2,1,1},

{8,4,
15,15,11,11,11,11,11,11,
14,14,14,14,15,15,15,15,
0,2,2,2,1,1,8,10,
2,0,0,3,4,1,8,10},

{8,4,
2,2,0,6,5,2,7,9,
2,2,2,2,0,0,8,10,
0,0,0,0,0,0,8,9,
0,0,0,3,4,0,7,9},

{8,4,
1,0,0,6,5,1,8,10,
1,1,0,1,0,1,8,9,
11,11,11,11,11,11,15,15,
14,14,14,15,15,15,15,14},

{8,4,
1,1,0,0,2,0,1,0,
0,0,3,4,0,1,1,0,
0,0,6,5,1,1,1,0,
1,1,0,0,1,1,2,0},

{8,4,
2,1,1,1,1,1,1,1,
2,0,1,1,0,1,1,0,
0,2,1,0,0,0,0,0,
1,0,3,4,0,1,3,4},

{8,4,
2,1,6,5,0,1,6,5,
2,2,2,2,1,1,0,1,
0,2,8,15,11,11,12,12,
2,0,8,15,14,14,14,14},

{8,4,
4,0,8,10,0,0,1,2,
5,1,7,9,1,3,4,2,
1,1,7,10,0,6,5,2,
1,1,8,10,1,1,0,1},

{8,4,
1,2,8,10,1,0,0,1,
1,1,8,10,0,0,0,0,
1,1,7,9,1,1,1,1,
12,11,15,15,11,12,12,12},

{8,4,
14,14,14,14,14,14,14,14,
1,0,0,0,0,0,0,0,
1,3,4,1,1,1,3,4,
1,6,5,0,0,1,6,5},

{8,4,
0,0,0,0,0,1,0,0,
0,0,1,0,3,4,1,1,
0,1,0,1,6,5,0,1,
1,1,1,1,1,1,1,1},

{8,4,
1,1,0,0,1,1,1,0,
0,3,4,0,0,0,3,4,
0,6,5,0,1,1,6,5,
1,0,0,0,1,0,1,1},

{8,4,
0,0,1,0,0,0,0,0,
1,1,0,0,0,1,0,0,
2,1,1,0,1,0,1,1,
1,1,2,2,0,0,0,1},

{8,4,
0,2,0,2,0,0,0,1,
2,0,2,2,2,1,1,1,
2,0,2,2,2,1,0,0,
12,11,11,11,11,11,12,12},

{8,4,
14,14,14,14,14,14,15,15,
0,0,0,0,1,1,7,9,
1,0,1,0,0,0,7,10,
1,0,0,0,0,1,8,10},

{8,4,
0,0,0,0,0,0,7,9,
1,0,0,3,4,0,7,10,
1,1,0,6,5,0,7,9,
1,1,0,0,2,2,7,10},

{8,4,
0,1,2,0,0,2,7,9,
1,3,4,2,0,0,7,10,
1,6,5,0,0,1,8,10,
1,1,1,0,2,2,7,9},

{8,4,
1,1,2,2,0,1,7,10,
12,11,15,11,11,15,15,15,
14,14,15,14,14,14,14,14,
1,7,10,0,2,0,0,0},

{8,4,
1,7,9,2,2,2,2,3,
1,7,10,1,1,2,2,6,
0,8,10,1,1,2,2,1,
0,7,9,1,1,0,0,2},

{8,4,
0,7,10,0,1,0,0,3,
0,7,9,1,0,0,0,6,
12,15,10,0,0,0,1,1,
14,15,9,0,0,1,2,2},

{8,4,
0,7,10,0,1,1,2,3,
1,7,9,0,0,0,0,6,
1,7,10,0,0,0,0,2,
1,8,10,0,0,0,1,2},

{8,4,
0,7,9,1,0,0,0,3,
0,7,10,0,0,1,1,6,
1,7,9,1,1,0,0,1,
0,7,15,11,11,11,11,11},

{8,4,
0,8,15,14,14,14,14,14,
0,0,0,0,1,1,0,0,
0,0,0,0,0,0,0,0,
0,0,0,3,4,0,0,0},

{8,4,
0,0,0,6,5,1,1,0,
0,0,1,0,0,0,1,0,
0,0,0,0,0,3,4,2,
0,0,0,0,0,6,5,0},

},

{

{8,4,
2,2,0,2,2,2,2,2,
0,3,4,2,2,0,0,3,
1,6,5,0,2,1,2,6,
1,1,1,2,1,1,2,2},

{8,4,
12,12,11,11,12,11,11,11,
14,14,14,14,14,14,14,14,
1,1,0,0,1,1,1,1,
1,0,0,0,0,0,1,3},

{8,4,
1,0,0,1,0,0,0,6,
1,0,0,0,1,0,1,2,
1,0,0,3,4,2,3,4,
1,0,1,6,5,2,6,5},

{8,4,
0,0,0,1,1,2,1,1,
1,1,0,1,0,2,2,0,
12,12,15,11,11,11,11,11,
14,14,15,15,14,14,14,14},

{8,4,
0,0,8,10,2,2,0,1,
3,4,8,10,0,2,1,0,
6,5,8,10,2,2,3,4,
0,0,8,10,2,1,6,5},

{8,4,
1,1,8,10,0,0,0,0,
1,1,7,9,0,0,3,4,
0,0,7,10,0,0,6,5,
1,0,8,10,0,0,2,2},

{8,4,
1,2,8,10,0,0,2,2,
2,2,8,10,0,1,2,2,
12,12,15,15,12,12,12,12,
14,14,14,14,14,14,14,14},

{8,4,
1,1,1,1,2,0,0,1,
3,4,1,0,0,0,1,1,
6,5,1,1,0,0,2,1,
1,0,1,0,0,0,0,1},

{8,4,
0,0,1,1,3,4,0,0,
0,0,1,0,6,5,0,0,
1,1,0,1,0,0,0,1,
12,11,9,1,1,0,0,0},

{8,4,
14,15,9,1,1,0,0,0,
1,7,10,1,1,1,1,1,
1,8,10,0,1,0,3,4,
0,8,10,0,0,0,6,5},

{8,4,
0,8,10,0,0,0,0,0,
0,7,9,1,1,0,1,1,
1,7,10,0,1,0,0,1,
0,8,10,1,1,0,1,1},

{8,4,
0,8,10,1,1,1,1,1,
0,8,15,11,12,11,11,11,
0,7,15,14,14,14,14,14,
0,0,1,0,1,0,1,0},

{8,4,
0,0,3,4,0,0,0,3,
0,1,6,5,0,1,0,6,
0,0,0,0,0,0,0,0,
0,0,1,1,1,0,0,0},

{8,4,
1,0,3,4,0,0,1,3,
0,0,6,5,1,0,2,6,
0,1,0,0,0,2,2,2,
12,12,11,11,12,11,12,11},

{8,4,
14,14,14,14,14,14,14,14,
1,1,0,2,2,2,2,2,
1,2,2,1,0,1,0,0,
1,2,3,4,0,0,2,1},

{8,4,
2,2,6,5,2,2,1,0,
0,2,2,2,2,1,1,1,
2,2,2,2,2,3,4,0,
0,2,0,2,0,6,5,0},

{8,4,
2,2,1,1,1,0,0,0,
1,2,1,2,1,1,0,0,
0,0,0,0,0,0,1,0,
3,4,0,0,0,0,0,0},

{8,4,
6,5,0,0,0,0,0,0,
11,12,12,12,12,11,11,12,
14,14,15,15,14,14,14,14,
0,0,7,9,0,0,1,1},

{8,4,
4,0,7,10,1,1,0,0,
5,1,8,10,0,0,0,0,
0,0,7,9,1,1,0,0,
0,0,7,10,0,0,1,0},

{8,4,
4,2,7,10,0,1,0,0,
5,0,7,9,0,0,1,1,
1,1,7,10,1,3,4,0,
1,1,8,10,0,6,5,0},

{8,4,
4,1,7,9,0,0,1,1,
5,0,7,10,1,0,1,0,
2,2,7,9,0,0,0,1,
2,2,7,10,0,0,0,1},

{8,4,
4,2,8,10,0,0,0,0,
5,2,7,9,2,2,2,2,
0,0,7,10,1,0,2,2,
11,12,15,15,15,11,11,12},

{8,4,
14,14,14,14,14,14,14,14,
0,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,
0,1,0,3,4,0,0,0},

{8,4,
0,0,1,6,5,1,1,0,
0,0,0,0,0,0,0,2,
0,0,0,0,2,3,4,2,
0,0,0,0,0,6,5,0},

},

{

{8,4,
2,0,0,0,0,1,1,1,
4,0,0,0,0,3,4,2,
5,1,1,1,0,6,5,2,
0,2,0,1,1,2,2,0},

{8,4,
11,11,12,12,12,12,11,11,
14,14,14,14,14,14,14,14,
0,0,0,0,0,0,0,0,
4,0,3,4,2,1,1,2},

{8,4,
5,0,6,5,2,1,1,0,
2,0,1,1,0,0,0,2,
1,1,1,1,3,4,0,0,
0,1,1,1,6,5,1,0},

{8,4,
0,0,0,0,1,1,1,1,
1,1,1,1,1,0,1,0,
11,11,11,11,11,12,10,0,
14,15,15,15,15,15,9,0},

{8,4,
1,1,1,0,1,8,10,1,
1,1,1,1,0,8,10,0,
0,3,4,0,1,7,15,11,
0,6,5,1,0,8,15,15},

{8,4,
0,0,2,0,1,8,10,0,
2,3,4,2,2,8,10,0,
0,6,5,2,0,7,9,0,
2,2,2,0,0,8,10,2},

{8,4,
0,0,0,2,2,8,9,0,
2,2,2,2,2,8,10,2,
11,12,12,12,12,15,10,0,
14,14,14,14,14,15,10,1},

{8,4,
0,0,0,0,1,8,10,1,
1,0,3,4,2,8,10,2,
1,1,6,5,2,8,10,2,
0,2,1,2,2,8,10,2},

{8,4,
0,0,1,1,2,7,9,2,
1,1,1,1,1,8,10,0,
2,0,2,0,0,8,9,0,
1,3,4,1,2,8,10,0},

{8,4,
0,6,5,0,2,8,10,0,
1,0,1,2,2,8,10,2,
0,1,1,0,0,7,9,0,
0,1,1,0,2,8,10,0},

{8,4,
0,1,0,0,0,8,9,0,
0,3,4,1,1,8,10,2,
0,6,5,1,1,7,10,2,
1,1,1,1,2,7,10,1},

{8,4,
1,1,1,2,1,8,10,1,
11,11,12,12,12,15,15,11,
14,14,14,14,14,14,14,14,
1,1,2,1,0,0,1,1},

{8,4,
4,1,2,3,4,0,1,1,
5,2,1,6,5,0,1,1,
2,2,0,1,0,1,0,1,
2,2,0,1,1,1,1,1},

{8,4,
4,1,1,3,4,0,0,0,
5,2,2,6,5,1,0,0,
1,0,0,0,0,0,0,0,
11,11,11,11,12,12,12,12},

{8,4,
14,14,14,15,15,14,14,14,
2,0,0,7,9,0,0,0,
3,4,1,7,10,1,0,0,
6,5,0,7,9,0,0,3},

{8,4,
2,1,0,7,10,0,0,6,
0,2,0,8,10,0,0,0,
1,1,0,8,10,0,1,0,
2,2,1,7,15,12,11,11},

{8,4,
2,2,1,7,15,15,14,14,
0,1,0,7,9,2,0,0,
3,4,1,7,10,2,0,0,
6,5,0,8,10,1,0,0},

{8,4,
0,0,0,7,9,1,0,1,
11,12,11,15,10,1,3,4,
14,14,15,15,9,0,6,5,
1,0,0,8,10,0,0,0},

{8,4,
3,4,0,7,9,1,0,0,
6,5,1,7,10,0,1,1,
0,2,0,7,9,0,0,2,
0,0,0,7,10,0,3,4},

{8,4,
0,0,0,8,10,2,6,5,
1,1,0,7,9,0,2,1,
0,0,1,7,10,0,0,0,
0,0,0,7,9,1,0,0},

{8,4,
1,1,0,8,10,1,0,1,
1,0,1,7,9,0,0,1,
1,1,1,7,15,12,11,11,
3,4,0,7,15,14,14,14},

{8,4,
6,5,1,7,10,3,4,1,
2,2,0,8,10,6,5,0,
1,2,0,7,9,0,0,0,
11,12,11,15,10,1,1,0},

{8,4,
14,14,14,15,9,1,1,1,
0,0,1,8,10,1,0,0,
1,1,1,7,9,1,0,0,
0,1,1,7,10,0,0,0},

{8,4,
0,1,2,7,9,2,2,0,
0,0,0,7,10,0,2,2,
0,0,0,8,10,2,0,0,
0,0,0,7,9,0,0,0},

},

{

{8,4,
1,7,10,1,0,1,0,1,
0,8,10,0,3,4,1,1,
2,8,10,0,6,5,0,0,
0,8,10,0,0,1,0,0},

{8,4,
11,15,15,11,11,11,11,10,
14,14,14,14,15,15,15,10,
2,2,0,2,0,0,7,10,
3,4,0,3,4,0,8,10},

{8,4,
6,5,0,6,5,0,8,10,
0,0,2,0,0,0,8,15,
0,2,0,0,0,2,7,15,
0,2,0,0,0,2,8,10},

{8,4,
1,1,1,1,3,4,8,9,
3,4,1,1,6,5,7,10,
6,5,1,1,0,2,8,10,
0,0,0,0,1,1,8,10},

{8,4,
1,0,0,0,0,0,8,10,
0,0,1,0,0,0,7,9,
11,11,12,12,12,12,15,10,
15,15,14,14,14,14,15,9},

{8,4,
0,1,0,0,1,0,8,10,
1,3,4,0,0,0,8,10,
0,6,5,1,1,0,8,10,
0,0,0,0,1,1,8,10},

{8,4,
0,0,3,4,1,0,7,9,
0,0,6,5,0,0,8,10,
0,0,1,1,0,1,8,9,
0,3,4,0,0,0,8,10},

{8,4,
2,6,5,1,0,1,7,10,
2,0,2,2,0,0,7,10,
2,2,3,4,2,2,8,15,
2,2,6,5,2,2,8,15},

{8,4,
2,2,0,0,0,2,0,1,
0,3,4,2,2,0,1,1,
0,6,5,0,2,2,1,1,
2,0,2,2,0,0,2,1},

{8,4,
0,0,0,3,4,0,0,0,
0,2,0,6,5,2,0,2,
3,4,1,0,0,0,0,0,
6,5,2,2,0,0,0,0},

{8,4,
0,0,0,1,0,0,0,0,
0,1,1,3,4,0,0,0,
1,1,0,6,5,0,0,1,
1,1,1,0,0,0,1,1},

{8,4,
1,1,1,1,0,1,1,0,
12,11,11,11,11,11,12,12,
14,14,14,14,14,14,14,14,
1,1,1,1,1,1,2,0},

{8,4,
0,0,0,0,3,4,2,1,
0,0,1,0,6,5,1,1,
1,1,0,0,0,0,0,0,
1,1,0,0,0,2,0,2},

{8,4,
1,0,0,0,3,4,2,0,
0,0,0,0,6,5,0,0,
1,0,0,0,0,0,0,1,
11,11,12,11,12,11,11,11},

{8,4,
14,14,14,14,14,14,14,14,
0,0,0,0,0,0,0,0,
1,1,0,1,0,0,0,0,
4,0,0,0,0,3,4,1},

{8,4,
5,1,0,1,0,6,5,0,
0,1,1,1,1,0,0,1,
1,1,0,0,2,0,0,1,
11,11,11,12,12,12,12,15},

{8,4,
14,14,14,14,14,14,14,15,
0,0,0,1,2,0,0,7,
0,3,4,1,0,0,1,7,
1,6,5,1,0,1,1,7},

{8,4,
1,1,2,2,2,0,1,7,
1,2,2,0,3,4,1,7,
0,2,2,2,6,5,1,7,
0,0,0,2,1,0,1,7},

{8,4,
1,3,4,2,2,1,0,7,
2,6,5,2,1,1,0,7,
0,2,0,1,1,0,0,7,
0,0,0,0,1,0,0,7},

{8,4,
2,0,0,1,2,2,2,7,
1,0,0,0,2,0,2,7,
2,3,4,0,0,3,4,7,
2,6,5,0,0,6,5,7},

{8,4,
1,2,2,1,1,1,2,7,
0,1,1,1,2,2,2,7,
11,11,11,12,12,12,12,15,
14,14,14,14,14,14,14,15},

{8,4,
1,1,0,1,1,0,1,7,
1,1,1,1,0,1,1,7,
0,0,1,3,4,1,1,7,
0,0,1,6,5,1,0,7},

{8,4,
0,1,1,0,0,0,0,7,
0,0,0,1,1,1,0,7,
0,0,0,1,2,2,1,1,
0,0,0,0,2,2,1,1},

{8,4,
3,4,0,0,0,0,0,1,
6,5,0,0,0,0,2,1,
2,2,0,0,2,2,2,1,
2,2,2,2,0,0,0,0},

},

{

{8,4,
1,1,1,1,0,0,0,0,
3,4,1,0,3,4,2,2,
6,5,0,1,6,5,0,2,
0,0,1,1,1,1,2,0},

{8,4,
0,0,3,4,1,0,2,0,
0,1,6,5,0,1,0,0,
0,1,1,1,1,1,3,4,
1,0,0,0,0,0,6,5},

{8,4,
0,0,1,0,0,0,0,0,
15,11,11,11,12,12,11,11,
14,14,14,14,14,14,14,14,
1,0,0,0,1,1,0,0},

{8,4,
1,0,0,0,1,1,0,0,
0,3,4,0,0,1,1,1,
2,6,5,2,1,2,2,1,
1,1,1,0,1,1,0,1},

{8,4,
2,0,1,0,0,3,4,0,
0,0,1,1,0,6,5,0,
0,0,1,1,0,0,1,0,
0,3,4,1,1,0,0,2},

{8,4,
1,6,5,1,0,0,0,1,
0,2,1,1,1,1,2,1,
0,2,2,2,1,3,4,1,
0,2,2,2,2,6,5,1},

{8,4,
1,2,2,2,0,0,0,1,
0,3,4,0,0,1,1,2,
0,6,5,2,2,1,1,2,
0,1,1,2,2,0,1,0},

{8,4,
0,0,0,1,1,1,1,1,
0,0,0,1,0,1,1,1,
11,11,11,11,12,12,12,12,
14,14,14,14,14,15,15,14},

{8,4,
0,1,1,0,0,8,10,1,
1,1,1,0,0,7,9,0,
3,4,1,0,0,8,10,1,
6,5,1,1,1,8,10,0},

{8,4,
0,0,1,0,0,7,9,1,
2,2,0,0,0,8,10,0,
3,4,0,0,0,8,9,0,
6,5,0,0,0,8,10,0},

{8,4,
0,0,1,0,0,8,10,0,
0,0,0,0,0,8,10,0,
3,4,0,1,1,7,9,0,
6,5,0,0,0,8,10,1},

{8,4,
1,1,1,1,0,8,9,0,
12,12,11,11,15,15,15,15,
14,14,14,14,14,14,14,15,
0,0,0,0,1,1,1,8},

{8,4,
2,0,0,0,1,1,2,8,
0,0,1,0,2,1,2,7,
2,2,3,4,0,0,0,7,
0,2,6,5,1,1,1,7},

{8,4,
0,0,0,0,1,0,0,7,
0,1,0,1,0,0,0,8,
1,1,0,0,0,0,0,8,
11,11,12,12,12,12,11,15},

{8,4,
14,14,14,14,14,14,14,15,
0,0,1,0,0,0,0,8,
1,1,0,0,0,1,0,8,
1,1,1,3,4,0,0,7},

{8,4,
1,1,1,6,5,2,2,8,
0,0,0,0,0,2,2,7,
0,0,0,1,0,2,0,7,
10,0,0,1,1,1,0,7},

{8,4,
9,1,1,1,2,1,1,7,
10,0,0,3,4,0,0,8,
10,0,0,6,5,0,0,7,
9,1,0,0,0,0,0,7},

{8,4,
10,0,0,1,0,0,0,7,
15,12,12,11,11,12,11,15,
15,14,14,14,14,14,14,14,
10,0,0,0,0,0,0,1},

{8,4,
10,0,0,0,0,0,0,2,
9,0,0,0,0,0,0,3,
10,0,0,0,0,0,1,6,
10,1,1,0,0,0,1,1},

{8,4,
9,0,0,1,0,0,0,0,
10,0,0,3,4,0,0,0,
9,0,1,6,5,1,0,0,
10,2,1,2,0,2,1,2},

{8,4,
9,2,2,2,2,2,2,2,
10,2,2,3,4,3,4,3,
9,0,0,6,5,6,5,6,
10,1,2,1,2,2,0,1},

{8,4,
9,1,1,1,1,0,0,1,
10,0,1,2,2,2,2,2,
10,1,1,1,0,2,2,2,
9,1,0,0,0,0,0,2},

{8,4,
15,12,11,11,11,11,11,12,
15,15,14,14,14,14,14,14,
0,0,1,1,1,0,1,1,
1,1,1,0,0,1,0,0},

{8,4,
3,4,1,2,3,4,1,0,
6,5,2,0,6,5,0,0,
1,0,0,0,0,0,1,0,
0,0,0,0,0,0,1,1},

},

{

{8,4,
0,0,0,0,0,0,0,0,
3,4,0,2,0,3,4,0,
6,5,2,2,2,6,5,1,
2,2,0,1,1,1,0,0},

{8,4,
2,0,3,4,2,0,0,2,
1,0,6,5,0,1,2,2,
1,1,1,1,1,2,3,4,
0,1,0,0,0,0,6,5},

{8,4,
0,0,0,0,0,0,2,0,
11,11,12,12,12,12,11,11,
14,15,15,14,14,14,14,14,
0,8,10,2,2,1,1,2},

{8,4,
0,7,9,0,2,1,1,0,
0,8,10,0,1,1,0,0,
1,8,10,2,1,1,0,0,
0,8,10,1,1,0,0,0},

{8,4,
2,7,9,0,0,3,4,0,
2,8,10,2,0,6,5,0,
2,8,9,2,0,0,0,0,
1,8,10,0,1,1,0,0},

{8,4,
1,8,10,1,1,1,0,0,
1,7,9,2,2,2,2,0,
2,8,10,1,2,3,4,0,
2,8,9,2,2,6,5,0},

{8,4,
0,8,10,2,1,0,0,1,
0,8,10,2,1,1,0,0,
0,8,10,1,1,1,1,0,
0,7,9,0,1,1,1,1},

{8,4,
1,8,10,1,1,1,1,1,
1,8,9,1,1,1,1,1,
11,15,15,11,11,11,12,12,
14,14,14,14,14,14,14,15},

{8,4,
1,0,1,1,0,0,1,1,
1,3,4,0,0,3,4,0,
0,6,5,1,1,6,5,0,
1,0,1,0,1,1,0,2},

{8,4,
1,0,0,0,1,2,0,0,
1,1,1,0,2,3,4,0,
0,1,1,1,2,6,5,0,
0,0,0,1,0,2,2,0},

{8,4,
0,1,0,2,1,2,1,0,
1,3,4,1,1,1,1,0,
0,6,5,2,1,1,1,0,
0,0,2,1,1,1,1,0},

{8,4,
2,2,1,1,1,0,1,1,
11,11,12,11,12,11,11,11,
15,15,14,14,14,14,14,14,
10,0,0,0,0,1,1,0},

{8,4,
10,0,3,4,1,1,1,1,
9,0,6,5,0,0,1,1,
10,1,1,1,0,1,1,0,
9,1,1,0,0,1,2,2},

{8,4,
10,1,0,0,1,1,1,0,
10,1,0,1,0,2,0,0,
10,0,3,4,1,2,1,1,
9,1,6,5,1,2,1,1},

{8,4,
10,0,0,0,1,1,0,2,
10,2,2,2,0,0,0,1,
10,2,0,2,0,0,0,0,
15,11,11,11,12,12,12,12},

{8,4,
15,14,14,14,14,14,14,14,
9,2,2,2,0,0,0,0,
10,2,1,0,1,0,0,0,
9,0,1,1,0,0,0,0},

{8,4,
10,0,0,1,1,1,3,4,
10,0,0,1,1,0,6,5,
9,0,0,0,1,0,1,0,
10,2,0,0,0,0,0,0},

{8,4,
9,2,2,1,0,1,1,0,
15,11,11,11,11,12,12,15,
14,14,14,14,14,14,14,15,
2,1,0,1,0,1,0,7},

{8,4,
0,0,1,0,0,0,0,7,
4,0,0,0,0,0,0,7,
5,0,0,0,0,0,1,8,
2,0,1,1,0,1,1,7},

{8,4,
0,1,1,0,0,0,0,7,
0,1,0,3,4,0,0,7,
0,0,0,6,5,0,0,7,
1,1,0,0,0,1,0,7},

{8,4,
1,0,0,0,0,1,1,7,
4,3,4,3,4,1,1,7,
5,6,5,6,5,1,1,7,
0,2,0,1,0,0,0,8},

{8,4,
1,2,2,1,1,0,0,7,
1,2,1,1,1,2,0,7,
2,2,2,0,2,2,0,7,
2,2,2,2,2,2,2,8},

{8,4,
12,12,12,12,12,12,12,15,
14,14,14,14,14,14,14,15,
0,0,0,2,2,2,2,7,
0,0,0,0,0,2,2,7},

{8,4,
0,0,1,0,3,4,0,8,
0,0,0,0,6,5,2,7,
0,1,0,0,0,0,0,7,
0,0,0,0,0,0,0,7},

},

{

{8,4,
0,0,1,1,0,0,1,1,
0,3,4,1,1,3,4,0,
1,6,5,1,0,6,5,0,
0,0,0,0,0,0,0,0},

{8,4,
2,8,15,11,11,12,12,12,
2,8,15,14,14,14,14,14,
0,7,9,0,0,0,0,0,
1,8,10,1,0,0,1,3},

{8,4,
0,8,9,0,1,1,0,6,
11,15,15,12,12,12,12,11,
14,14,14,14,15,15,14,14,
0,0,0,0,8,10,0,2},

{8,4,
0,0,0,0,7,9,0,3,
3,4,0,0,8,10,1,6,
6,5,0,0,8,9,0,0,
0,1,1,1,8,10,1,3},

{8,4,
0,1,1,0,8,10,0,6,
1,1,1,0,8,9,0,0,
0,0,0,0,8,10,0,0,
3,4,1,0,7,9,0,0},

{8,4,
6,5,1,1,8,15,11,11,
0,0,2,1,8,14,14,14,
0,1,1,0,8,10,0,0,
1,1,1,0,7,9,0,3},

{8,4,
1,1,1,1,8,10,2,6,
3,4,1,2,8,9,2,2,
6,5,2,2,8,10,0,0,
1,1,1,0,8,10,0,1},

{8,4,
1,0,0,0,7,9,0,1,
1,1,0,0,8,10,1,1,
12,12,11,11,15,9,0,0,
15,15,14,15,15,10,1,3},

{8,4,
7,9,1,2,2,2,2,6,
8,10,1,1,0,2,0,2,
8,10,2,1,1,2,1,1,
7,15,11,11,11,11,12,12},

{8,4,
8,15,14,14,14,14,14,14,
0,0,0,1,0,1,0,1,
0,1,0,0,0,0,3,4,
1,1,1,0,0,1,6,5},

{8,4,
0,0,0,0,0,0,0,1,
0,0,1,0,0,1,0,0,
1,0,1,1,1,1,1,0,
1,0,0,0,1,0,0,0},

{8,4,
1,0,0,0,1,0,0,0,
11,11,12,12,12,12,11,12,
14,14,14,14,14,14,14,14,
0,1,0,1,0,1,2,2},

{8,4,
0,1,3,4,0,0,1,1,
1,0,6,5,1,0,1,1,
0,0,0,0,0,1,0,1,
1,1,2,2,1,1,2,7},

{8,4,
1,0,1,2,1,1,0,7,
2,0,1,2,1,1,0,7,
2,0,3,4,2,0,0,7,
2,2,6,5,1,0,0,7},

{8,4,
0,0,0,0,1,0,0,7,
0,0,0,0,0,0,0,7,
0,0,1,0,0,0,0,7,
11,11,12,11,12,11,11,15},

{8,4,
14,14,14,14,14,14,15,15,
0,2,1,0,2,2,2,7,
1,2,2,2,2,1,0,7,
0,0,0,0,1,0,0,7},

{8,4,
0,1,1,1,3,4,0,7,
0,1,1,0,6,5,0,7,
0,0,0,0,0,1,0,7,
0,0,0,0,0,1,0,7},

{8,4,
0,0,1,0,0,2,0,7,
10,1,1,3,4,2,1,7,
9,0,0,6,5,2,2,7,
10,0,0,1,0,0,2,7},

{8,4,
9,0,1,1,1,0,0,7,
10,0,0,1,1,0,2,7,
10,0,0,0,2,2,2,7,
9,0,0,3,4,0,0,7},

{8,4,
10,0,0,6,5,2,2,7,
9,0,2,0,2,0,0,7,
15,12,11,11,12,11,12,15,
15,14,14,14,14,14,14,14},

{8,4,
10,0,0,0,0,0,2,0,
9,0,3,4,0,0,2,2,
10,0,6,5,0,0,2,2,
10,0,0,0,0,0,0,0},

{8,4,
9,0,0,0,0,0,0,0,
10,2,2,2,3,4,0,0,
9,2,2,2,6,5,0,0,
10,2,0,2,0,0,2,0},

{8,4,
15,12,11,11,11,11,11,12,
15,15,14,14,14,14,14,14,
9,0,0,0,0,0,0,1,
10,0,1,0,0,0,1,0},

{8,4,
10,0,1,3,4,0,0,1,
9,2,2,6,5,1,1,1,
10,2,2,0,0,0,0,0,
9,2,0,0,0,0,2,0},

},

{

{8,4,
1,1,0,0,0,0,1,0,
0,0,3,4,1,0,0,3,
0,0,6,5,1,0,0,6,
0,0,0,0,0,0,0,1},

{8,4,
12,11,12,11,11,11,11,11,
14,14,14,14,14,14,14,14,
0,0,0,0,1,0,0,1,
4,0,0,0,0,3,4,0},

{8,4,
5,0,1,1,1,6,5,0,
11,11,11,11,12,12,12,12,
14,14,14,14,14,14,14,14,
2,0,0,1,0,0,1,0},

{8,4,
4,0,0,0,0,3,4,0,
5,0,0,0,0,6,5,0,
1,0,1,1,2,2,1,0,
4,1,1,2,1,0,1,2},

{8,4,
5,0,1,1,0,0,0,1,
1,0,3,4,0,0,0,0,
0,0,6,5,0,0,0,0,
0,0,0,0,0,0,0,0},

{8,4,
11,11,12,12,12,12,11,15,
14,14,14,14,14,14,14,14,
2,0,0,0,0,0,0,0,
4,0,0,3,4,0,2,2},

{8,4,
5,2,2,6,5,0,0,0,
2,2,2,1,2,2,0,0,
2,2,2,1,2,1,1,0,
1,0,0,0,0,1,1,0},

{8,4,
1,1,3,4,0,0,0,0,
0,0,6,5,0,0,0,0,
0,0,0,0,0,0,0,0,
4,1,1,0,0,0,0,0},

{8,4,
5,1,0,0,0,3,4,0,
0,1,1,1,1,6,5,1,
1,0,1,1,1,1,1,1,
12,12,11,12,12,12,11,12},

{8,4,
14,14,14,14,14,14,14,14,
0,1,1,1,0,0,0,0,
0,0,0,0,1,3,4,1,
1,0,1,1,0,6,5,0},

{8,4,
1,0,0,0,0,0,0,0,
0,0,0,0,0,2,2,2,
1,3,4,1,2,1,2,2,
0,6,5,0,1,1,2,2},

{8,4,
1,2,2,2,2,1,2,1,
11,11,11,11,11,12,12,12,
14,14,14,14,14,15,15,14,
2,0,0,0,1,8,10,1},

{8,4,
2,0,3,4,1,7,9,1,
0,0,6,5,0,7,10,1,
0,0,0,1,0,7,9,2,
15,11,11,11,11,15,15,12},

{8,4,
15,14,14,14,14,14,14,14,
9,0,0,0,0,0,1,0,
10,0,3,4,0,0,0,0,
10,0,6,5,1,0,0,0},

{8,4,
9,0,0,0,1,3,4,0,
10,1,0,0,0,6,5,0,
9,1,0,0,0,2,2,2,
15,11,11,11,12,12,12,12},

{8,4,
15,15,14,14,14,14,14,14,
9,0,2,0,2,1,2,2,
10,0,1,0,0,0,2,2,
10,1,1,1,0,0,0,0},

{8,4,
9,0,1,1,0,0,0,0,
10,1,1,0,0,0,3,4,
9,0,0,0,0,0,6,5,
10,0,3,4,0,0,0,0},

{8,4,
9,0,6,5,0,1,0,0,
10,1,1,1,0,1,1,7,
10,1,1,0,0,1,0,7,
9,0,1,1,1,0,0,7},

{8,4,
10,0,1,1,1,0,1,7,
10,0,0,0,0,0,0,7,
9,0,1,3,4,0,0,8,
10,0,0,6,5,0,1,7},

{8,4,
10,0,0,0,1,0,1,7,
9,2,1,1,0,1,1,7,
15,15,11,11,12,12,11,15,
14,14,14,14,14,14,14,14},

{8,4,
2,2,2,1,2,0,0,0,
2,0,1,3,4,0,0,0,
2,0,0,6,5,0,0,0,
2,2,1,2,1,0,2,2},

{8,4,
0,0,0,0,0,0,2,0,
0,3,4,0,2,2,2,2,
0,6,5,1,2,2,2,2,
0,2,2,2,0,0,2,0},

{8,4,
12,12,11,11,11,11,11,12,
14,14,14,14,14,14,14,14,
1,1,1,1,0,0,0,0,
1,0,1,0,3,4,0,0},

{8,4,
0,3,4,1,6,5,1,0,
1,6,5,1,0,0,0,0,
0,0,1,0,0,0,0,0,
0,0,0,1,0,0,1,0},

},

{

{8,4,
0,1,0,0,0,0,0,8,
4,0,0,0,0,0,0,8,
5,0,0,0,1,1,0,7,
1,1,0,1,1,0,1,8},

{8,4,
12,12,12,12,11,12,11,15,
14,14,14,14,14,14,14,15,
0,0,1,0,0,0,0,0,
0,0,0,3,4,0,0,1},

{8,4,
1,0,0,6,5,0,0,1,
11,10,1,0,0,0,0,0,
15,10,1,3,4,1,1,1,
8,10,0,6,5,0,1,0},

{8,4,
7,9,0,0,0,0,0,0,
8,10,1,0,0,0,0,0,
8,15,15,11,11,11,12,12,
8,15,14,14,14,14,14,14},

{8,4,
8,10,0,1,0,0,1,1,
7,9,0,2,2,0,2,1,
8,10,0,0,3,4,2,2,
8,10,1,2,6,5,2,2},

{8,4,
15,10,2,0,0,2,2,2,
15,10,2,2,2,2,2,2,
8,10,0,0,0,0,0,0,
7,9,0,0,0,0,0,0},

{8,4,
8,10,0,0,0,0,0,1,
8,10,0,0,0,0,0,0,
8,10,0,0,3,4,1,0,
7,9,0,0,6,5,0,0},

{8,4,
8,10,0,0,0,0,1,0,
8,9,0,0,0,0,0,0,
8,10,0,0,1,1,0,0,
8,10,0,0,1,1,1,0},

{8,4,
7,9,0,0,0,1,1,0,
8,10,1,0,0,0,0,0,
8,10,0,0,0,2,2,1,
15,10,0,0,3,4,2,1},

{8,4,
15,9,0,2,6,5,2,1,
8,10,0,0,2,2,2,2,
8,9,1,1,2,2,0,1,
8,10,0,2,0,2,1,1},

{8,4,
8,10,0,2,2,0,1,0,
8,10,1,0,0,0,0,1,
7,9,1,1,0,0,0,0,
8,10,1,1,1,1,0,0},

{8,4,
8,9,0,0,0,0,2,2,
15,15,11,11,11,11,11,12,
14,14,14,14,14,14,14,14,
0,2,2,2,2,2,1,0},

{8,4,
3,4,2,2,2,3,4,1,
6,5,2,1,1,6,5,0,
1,2,0,0,0,0,0,1,
12,11,11,12,11,12,11,11},

{8,4,
14,14,14,14,14,14,14,14,
0,0,0,2,0,0,0,0,
1,1,2,0,0,0,3,4,
1,1,0,1,1,0,6,5},

{8,4,
0,2,2,1,1,0,0,0,
0,1,0,1,1,0,0,0,
0,2,0,2,0,0,0,0,
11,11,12,11,12,11,11,11},

{8,4,
14,14,14,14,14,14,14,14,
2,0,0,0,0,0,0,0,
0,0,0,0,3,4,0,0,
0,0,0,0,6,5,0,0},

{8,4,
0,0,0,0,0,0,0,1,
0,0,0,3,4,0,0,0,
0,0,0,6,5,0,0,0,
0,0,0,0,1,0,2,2},

{8,4,
0,0,0,0,0,0,2,2,
15,12,11,11,12,11,12,11,
15,14,14,14,14,14,14,14,
10,2,2,2,0,2,2,2},

{8,4,
9,1,3,4,1,3,4,0,
10,1,6,5,2,6,5,1,
10,1,1,0,0,0,0,0,
9,0,3,4,2,0,3,4},

{8,4,
10,0,6,5,2,0,6,5,
9,1,0,1,2,1,1,1,
15,11,12,11,11,11,11,11,
14,14,14,14,14,14,14,14},

{8,4,
0,1,1,1,1,1,1,0,
0,0,0,3,4,0,0,0,
0,0,0,6,5,0,0,0,
2,0,0,1,1,1,1,0},

{8,4,
0,0,0,0,0,2,0,1,
0,3,4,1,0,1,2,2,
0,6,5,2,1,1,2,2,
1,0,2,2,1,1,1,1},

{8,4,
11,11,11,11,11,12,12,12,
14,14,14,14,14,14,14,14,
1,0,1,1,2,2,0,1,
1,1,3,4,0,2,1,0},

{8,4,
0,2,6,5,0,3,4,1,
0,2,0,2,2,6,5,2,
1,0,0,1,0,2,2,2,
0,1,2,0,0,2,0,0},

},

{

{8,4,
10,0,0,0,0,0,0,0,
10,1,1,3,4,1,1,0,
9,1,0,6,5,1,0,1,
10,1,1,1,1,0,0,0},

{8,4,
9,1,0,0,0,3,4,0,
10,1,0,0,0,6,5,0,
0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,0},

{8,4,
1,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,
0,3,4,0,0,3,4,0,
0,6,5,0,0,6,5,1},

{8,4,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
12,12,11,12,11,11,11,11,
14,14,14,14,14,14,14,14},

{8,4,
1,0,0,0,0,0,0,1,
2,2,1,0,1,0,0,1,
1,1,2,0,3,4,0,0,
1,1,2,2,6,5,0,0},

{8,4,
2,0,2,0,0,1,0,0,
2,0,2,2,0,1,0,1,
3,4,2,2,2,2,0,0,
6,5,1,0,2,2,2,1},

{8,4,
2,2,2,0,1,1,2,0,
0,2,2,0,1,0,0,0,
1,1,1,0,3,4,0,0,
1,1,1,0,6,5,2,2},

{8,4,
0,0,0,2,2,1,0,0,
1,1,2,2,1,2,0,2,
3,4,1,0,0,1,0,2,
6,5,0,0,0,0,1,0},

{8,4,
0,0,0,0,0,0,0,1,
2,0,0,0,0,0,0,0,
1,0,1,0,0,0,0,0,
1,1,0,0,3,4,0,0},

{8,4,
1,1,1,0,6,5,1,0,
0,0,0,0,0,0,1,0,
0,1,1,0,1,0,1,0,
0,1,1,2,0,2,1,0},

{8,4,
1,2,2,0,1,1,1,0,
3,4,2,0,0,0,0,0,
6,5,0,0,0,0,0,0,
0,2,0,0,0,0,1,1},

{8,4,
2,0,1,1,1,1,1,1,
12,12,12,11,12,11,11,11,
15,15,15,14,14,14,14,14,
8,10,0,0,0,1,1,0},

{8,4,
8,10,0,1,3,4,0,0,
7,9,0,0,6,5,0,0,
7,10,0,0,1,0,1,0,
15,9,0,1,1,1,0,0},

{8,4,
15,10,1,1,1,1,0,0,
1,0,0,0,0,0,0,0,
1,0,1,0,3,4,1,0,
1,1,0,0,6,5,0,0},

{8,4,
2,1,1,2,2,0,2,2,
1,1,1,0,2,2,1,2,
1,0,1,0,2,2,2,2,
11,11,11,12,12,12,12,11},

{8,4,
14,14,14,14,14,14,14,14,
0,0,2,1,1,1,0,0,
0,2,2,2,0,1,0,1,
0,0,0,2,2,1,0,1},

{8,4,
0,0,3,4,1,0,0,1,
1,2,6,5,1,0,0,1,
1,1,0,1,1,2,1,1,
1,0,0,1,2,2,1,2},

{8,4,
2,1,0,2,2,1,1,2,
12,12,11,11,12,11,12,11,
14,14,14,14,14,14,14,14,
1,0,0,0,0,1,0,0},

{8,4,
1,3,4,1,0,3,4,0,
1,6,5,1,1,6,5,0,
1,1,1,1,1,1,0,0,
0,1,3,4,1,1,3,4},

{8,4,
0,1,6,5,0,0,6,5,
0,1,0,0,0,0,0,0,
12,15,10,0,1,1,1,0,
15,15,9,1,1,1,0,1},

{8,4,
0,7,10,1,1,0,1,0,
0,7,10,1,1,0,1,1,
1,7,9,0,1,1,0,0,
0,7,15,12,11,11,11,11},

{8,4,
0,8,15,14,14,14,14,14,
0,7,9,1,1,0,0,0,
1,7,10,1,3,4,1,1,
1,7,9,1,6,5,1,1},

{8,4,
15,15,10,1,0,0,0,1,
14,15,9,1,0,0,0,1,
1,7,10,0,0,0,1,0,
1,7,9,0,0,0,0,0},

{8,4,
2,7,10,1,3,4,0,1,
2,8,10,0,6,5,1,1,
2,7,9,0,0,0,2,2,
2,7,10,2,1,0,1,0},

}
};

const char title_map[] PROGMEM =
{21,4,
 8, 7, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 5, 6,13,15,17,19,18,22,15,26,13,15,17,15,14,15,25,13,15,13,10, 
 3, 4,12,12,12,20,12,12,12,23,22,11,12,12,13,24,12,12,12,12, 0, 
 2, 1, 9,11,16, 0,16,21,11,16, 9,10,16,16, 9,11,16, 9,11,16, 0
};