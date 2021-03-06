/*
 *  Uzebox Kernel
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

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "defines.h"
#include "uzebox.h"

#define CHAR_ZERO 16 

extern unsigned char overlay;
extern char scanline_sprite_buf[];
extern struct SpriteStruct sprites[];
extern char sprites_per_lines[200][4];
extern unsigned char scroll_x;
extern unsigned char scroll_x_offset;
extern unsigned char scroll_y;

#include "data/scrolltable.inc"

/*
Draws a map of tile at the specified position
*/
void DrawMap(unsigned char x,unsigned char y,const int *map){
	int i;
	int mapWidth=pgm_read_word(&(map[0]));
	int mapHeight=pgm_read_word(&(map[1]));

	for(unsigned char dy=0;dy<mapHeight;dy++){
		for(unsigned char dx=0;dx<mapWidth;dx++){
			
			i=pgm_read_word(&(map[(dy*mapWidth)+dx+2]));
			
			SetTile(x+dx,y+dy,i);	
		}
	}

}

/*
Draws a map of tile at the specified position
*/
void DrawMap2(unsigned char x,unsigned char y,const char *map){
	unsigned char i;
	unsigned char mapWidth=pgm_read_byte(&(map[0]));
	unsigned char mapHeight=pgm_read_byte(&(map[1]));

	for(unsigned char dy=0;dy<mapHeight;dy++){
		for(unsigned char dx=0;dx<mapWidth;dx++){
			
			i=pgm_read_byte(&(map[(dy*mapWidth)+dx+2]));
			
			//SetTile(x+dx,y+dy,i);	
			vram[((y+dy)*32)+x+dx]=i;
		}
	}

}

//Print an unsigned long in decimal
void PrintLong(int x,int y, unsigned long val){
	unsigned char c,i;

	for(i=0;i<6;i++){
		c=val%10;
		if(val>0 || i==0){
			SetFont(x--,y,c+CHAR_ZERO);
		}else{
			SetFont(x--,y,0);
		}
		val=val/10;
	}
		
}

//Print a byte in decimal
void PrintByte(int x,int y, unsigned char val){
	unsigned char c,i;

	for(i=0;i<3;i++){
		c=val%10;
		if(val>0 || i==0){
			SetFont(x--,y,c+CHAR_ZERO);
		}else{
			SetFont(x--,y,0);
		}
		val=val/10;
	}
		
}

//Print a byte in binary format
void PrintBinaryByte(char x,char y,unsigned char byte){
	int i;
	
	for(i=0;i<8;i++){
		
		if(byte&0x80){
			SetFont(x+i,y,CHAR_ZERO+1);
		}else{
			SetFont(x+i,y,CHAR_ZERO);
		}
		byte<<=1;
	}
}

//Print an unsigned byte in decimal
void PrintInt(int x,int y, unsigned int val,bool zeropad){
	unsigned char c,i;

	for(i=0;i<5;i++){
		c=val%10;
		if(val>0 || i==0){
			SetFont(x--,y,c+CHAR_ZERO);
		}else{
			if(zeropad){
				SetFont(x--,y,CHAR_ZERO);
			}else{
				SetFont(x--,y,0);
			}
		}
		val=val/10;
	}
		
}

//Print a byte in hexadecimal
void PrintHexByte(char x,char y,unsigned char byte){
	unsigned char nibble;

	//hi nibble	
	nibble=(byte>>4);
	if(nibble<=9){
		SetFont(x,y,nibble+CHAR_ZERO);
	}else{
		SetFont(x,y,nibble+CHAR_ZERO+7);
	}

	//lo nibble	
	nibble=(byte&0xf);
	if(nibble<=9){		
		SetFont(x+1,y,nibble+CHAR_ZERO);
	}else{
		SetFont(x+1,y,nibble+CHAR_ZERO+7);
	}

}

//Print a hexdecimal integer
void PrintHexInt(char x,char y,int value){
	PrintHexByte(x,y, (unsigned int)value>>8);
	PrintHexByte(x+2,y,value&0xff);
}

//Print a string
void Print(int x,int y,const char *string){


	int i=0;
	char c;

	while(1){
		c=pgm_read_byte(&(string[i++]));		
		if(c!=0){
			c=(c&127)-32;			
			SetFont(x++,y,c);
		}else{
			break;
		}
	}
	
}

//Print a single character
void PrintChar(int x,int y,char c){

	SetFont(x,y,(c-32));
	
}

//Wait for the beginning of next frame (30hz)
void WaitVsync(int count){
	int i;
	for(i=0;i<count;i++){
		while(!GetVsyncFlag());
		ClearVsyncFlag();
	}
}

//fill a region with the specified tile from the tile table
void Fill(int x,int y,int width,int height,int tile){
	int cx,cy;
	
	for(cy=0;cy<height;cy++){
		for(cx=0;cx<width;cx++){		
			SetTile(x+cx,y+cy,tile);
		}
	}
}

//fill a region with the specified tile from the font table
void FontFill(int x,int y,int width,int height,int tile){
	int cx,cy;
	
	for(cy=0;cy<height;cy++){
		for(cx=0;cx<width;cx++){		
			SetFont(x+cx,y+cy,(tile-32));
		}
	}
}


#if VIDEO_MODE == 2

	/**
	 * Scroll the window to the specified position within the 
	 * 32x32 tiles VRAM. Note that due to the non-binary aligment of the 
	 * tiles width(6), the caller needs to perform X wrapping
	 * before calling this function. Wrapping is at X_SCROLL_WRAP 
	 * (32 horizontal VRAM tiles * 6 horizontal pixel per tiles = 192)
	 * Y Scrolling doesnt need this because it wraps automatically at 0xff.
	 * Try to execute this call a close as possible to WaitVsync() call
	 * to minimize chances that a rendering interrupt occurs between the setting
	 * of scroll_x and scroll_y.
	 *
	 * I.e:
	 *
	 * sx++;
	 * if(sx>=X_SCROLL_WRAP) sx=0;
	 *
	 * or
	 *
	 * sx--;
	 * if(sx==0xff) sx=(X_SCROLL_WRAP-1);
	 */
	void SetScrolling(unsigned char sx,unsigned char sy){
		
		//prevent srolling overflow
		if(sx>=0 && sx<X_SCROLL_WRAP){
			unsigned char c1=pgm_read_byte(&(scrolltable[sx][0]));
			unsigned char c2=pgm_read_byte(&(scrolltable[sx][1]));
			scroll_x=c1;
			scroll_x_offset=c2;
		}

		scroll_y=sy;
	}

	/**
	 * Computes the sprites visibily per scanline. This
	 * function is invoked for each field at the beginning of VSYNC.
	 *
	 * This function is used internally by the kernel. User programs
	 * should not invoke it directly.
	 */
	void ProcessSprites(){		
		unsigned char sprNo,cy,yclip=0,sx,sy,disp;		

		//check if overlay has priority
		if((overlay&0x80) && (overlay&0x0f)){
			yclip=((overlay&0x0f)+1)*8;
		}else{
			yclip=8; 
		}

		
		//erase the sprites-per-line buffer
		for(sy=0;sy<(SCREEN_TILES_V*TILE_HEIGHT);sy++){
			sprites_per_lines[sy][0]=0;
			sprites_per_lines[sy][1]=0;
			sprites_per_lines[sy][2]=0;
			sprites_per_lines[sy][3]=0;
		}

		//fill buffer
		for(sprNo=1;sprNo<MAX_SPRITES;sprNo++){
			sy=sprites[sprNo].y;
			sx=sprites[sprNo].x;

			//ignore sprites that are totally off screen
			if(sy>0 && sy<((SCREEN_TILES_V+1)*TILE_HEIGHT) && sx>0 && sx<((SCREEN_TILES_H+1)*TILE_WIDTH)){
		
				for(cy=0;cy<TILE_HEIGHT;cy++){
					if((sy+cy >= yclip) && (sy+cy < ((SCREEN_TILES_V+1)*TILE_HEIGHT)) ){

						disp=sy+cy-8;

						//find free slot for sprite row otherwise it cannot be displayed
						if(sprites_per_lines[disp][0]==0){
							sprites_per_lines[disp][0]=(cy<<5)+sprNo;

						}else if(sprites_per_lines[disp][1]==0){
							sprites_per_lines[disp][1]=(cy<<5)+sprNo;
			
						}else if(sprites_per_lines[disp][2]==0){
							sprites_per_lines[disp][2]=(cy<<5)+sprNo;
			
						}else if(sprites_per_lines[disp][3]==0){
							sprites_per_lines[disp][3]=(cy<<5)+sprNo;
						}

					}

				}
	
			}	
		}


	}
#endif

