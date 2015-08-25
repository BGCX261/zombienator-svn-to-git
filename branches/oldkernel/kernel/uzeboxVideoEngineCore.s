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

/*
	V2 Changes:
	-Sprite engine
	-Reset console with joypad
	-ReadJoypad() now return int instead of char
	-NTSC timing more accurate
	-Use of conditionals (see defines.h)
	-Many small improvements

*/

#include <avr/io.h>
#include "defines.h"

;Line rate timer delay: 15.73426 kHz*2 = 1820/2 = 910
;2x is to account for vsync equalization & serration pulse that are at 2x line rate

#define HDRIVE_CL 1819
#define HDRIVE_CL_TWICE 909 
#define SYNC_HSYNC_PULSES 253

#define SYNC_PRE_EQ_PULSES 6
#define SYNC_EQ_PULSES 6
#define SYNC_POST_EQ_PULSES 6

#define SYNC_PHASE_PRE_EQ	0
#define SYNC_PHASE_EQ		1
#define SYNC_PHASE_POST_EQ	2
#define SYNC_PHASE_HSYNC	3

#define SYNC_PIN PB0
#define VIDEOCE_PIN PB4
#define SYNC_PORT PORTB
#define DATA_PORT PORTC


#define JOYPAD_OUT_PORT PORTA
#define JOYPAD_IN_PORT PINA
#define JOYPAD_CLOCK_PIN PA3
#define JOYPAD_LATCH_PIN PA2
#define JOYPAD_DATA1_PIN PA0
#define JOYPAD_DATA2_PIN PA1

;Public methods
.global TIMER1_COMPA_vect
.global InitVideo
.global SetTile
.global SetFont
.global RestoreTile
.global LoadMap
.global ClearVram

.global GetVsyncFlag
.global ClearVsyncFlag
.global DisplayCursor
.global MoveCursor
.global InitCursor
.global SetTileTable
.global SetFontTable
.global SetTileMap
.global read_joypads


;Public variables
.global vram
.global sync_pulse
.global sync_phase
.global hsync_align

.global joypad1_status_lo
.global joypad2_status_lo


#if VIDEO_MODE == 2
.global scanline_sprite_buf
.global	sprites_per_lines
.global	sprites
.global scroll_x
.global scroll_x_offset
.global scroll_y
.global SetOverlay
.global SetSpritesTileTable
.global overlay
#endif

#if VRAM_ADDR_SIZE == 1
.global SetFontTilesIndex
#endif

.section .bss
	#if VIDEO_MODE == 2
		.align 5
	#endif
	vram: 	  				.space VRAM_SIZE ;MUST be aligned to 32 bytes
	#if VIDEO_MODE == 2
		overlay_vram:		.space VRAM_TILES_H*OVERLAY_HEIGHT ;IMPORTANT: MUST be aligned to 32 bytes and contiguous to VRAM
		overlay:   		    .byte 1	;b7    Priority: 0=overlay is below sprites, 1=overlay above sprites
									;b6    Location: 0=top, 1=bottom (not implemented yet)
									;b5:b4 Reserved
									;b3:b0 Height in tiles (0-15). 0=overlay off
		
		playfield_start_lo: .byte 1
		playfield_start_hi: .byte 1

		scanline_sprite_buf:.space (SCREEN_TILES_H+2)*TILE_WIDTH ;+2 to account for left+right clipping
		sprites_per_lines:	.space SCREEN_TILES_V*TILE_HEIGHT*4  ;+2 for top+bottom clipping. |Y-offset(3bits)|Sprite No(5bits)|
			
		sprites:			.space 32*SPRITE_STRUCT_SIZE ;|X|Y|TILE INDEX|
		sprite_buf_erase:	.space 4*2; ;4x16 bit pointers
		sprites_tiletable_lo: .space 1
		sprites_tiletable_hi: .space 1
		scroll_x_offset:	.byte 1
		scroll_x:			.byte 1
		scroll_y:			.byte 1
	#endif

	sync_phase:  .byte 1 ;0=pre-eq, 1=eq, 2=post-eq, 3=hsync, 4=vsync
	sync_pulse:	 .byte 1
	vsync_flag:	 .byte 1	;set 30 hz
	curr_field:	 .byte 1	;0 or 1, changes at 60hz

	tile_table_lo:	.byte 1
	tile_table_hi:	.byte 1
	tile_map_lo:	.byte 1
	tile_map_hi:	.byte 1
	font_table_lo:	.byte 1
	font_table_hi:	.byte 1

	#if VRAM_ADDR_SIZE == 1
		font_tile_index:.byte 1 	
	#endif 

	;last read results of joypads	
	joypad1_status_lo:	.byte 1
	joypad1_status_hi:	.byte 1
	joypad2_status_lo:	.byte 1
	joypad2_status_hi:	.byte 1

	hsync_align: .byte 1

.section .init8
	call InitSound
	call InitVideo
	call InitConsole


.section .text
	
	sync_func_vectors:	.word pm(do_pre_eq)
						.word pm(do_eq)
						.word pm(do_post_eq)
						.word pm(do_hsync)
#if VIDEO_MODE == 2
	
	mode2_render_offsets:
						.word pm(mode2_render_line_offset_0)
						#if MODE2_HORIZONTAL_SCROLLING == 1
						.word pm(mode2_render_line_offset_1)
						.word pm(mode2_render_line_offset_2)
						.word pm(mode2_render_line_offset_3)
						.word pm(mode2_render_line_offset_4)
						.word pm(mode2_render_line_offset_5)
						#endif
#endif



#if VIDEO_MODE == 2

;***************************************************
; Mode 2: Tile & Sprites Video Mode
; Process video frame in tile mode (24*28) 
; -with sprites
;***************************************************	

sub_video_mode2:

	;waste line to align with next hsync in render function
	ldi ZL,216
m2_render_delay:
	lpm
	nop
	dec ZL
	brne m2_render_delay 

	

	ldi YL,lo8(vram)
	ldi YH,hi8(vram)

	;add X scrolling offset (rough)
	clr r0

	#if MODE2_HORIZONTAL_SCROLLING == 1
		lds r16,scroll_x
		add YL,r16
		adc YH,r0
	#else
		rjmp .
		rjmp .
	#endif
	
	;save Y scroll wrap adress (used as temp registers)
	out _SFR_IO_ADDR(GPIOR1),YL 
	out _SFR_IO_ADDR(GPIOR2),YH

	;add Y scrolling offset 
	lds r18,overlay
	andi r18,0x7	;keep 3 LSBits
	mov r16,r18
	lsl r18		;*TILE_HEIGHT
	lsl r18
	lsl r18

	lds r23,scroll_y
	add r23,r18		;add overlay height
	mov r22,r23
	mov r2,r22
	andi r22,0x7	;tile offset
	lsr r2
	lsr r2
	lsr r2
	ldi r17,VRAM_TILES_H
	mul r2,r17
	add YL,r0
	adc YH,r1
	sts playfield_start_lo,YL ;save 
	sts playfield_start_hi,YH



	
	clr r14	;set playfield rendering
	
	ldi XL,lo8(overlay_vram)
	ldi XH,hi8(overlay_vram)
	clr r0
	cpse r18,r0	;overlay is on (lines>0)?
	movw YL,XL ;start rendering with overlay
	cpse r18,r0
	mov r15,r23 ;save y scroll line
	cpse r18,r0	
	mov r9,r22  ;save playfield tile offset
	cpse r18,r0	
	clr r22		;no tile row offset for overlay
	cpse r18,r0	
	inc r14		;set overlay rendering

	ldi r18,0x80
	sub r18,r16 ;to create overflow after last overlay line
	

	ldi r16,SCREEN_TILES_V*TILE_HEIGHT; total scanlines to draw (28*8)
	mov r10,r16

	//ldi XL,lo8(sprites_per_lines+(TILE_HEIGHT*4)) //top clipping offset
	//ldi XH,hi8(sprites_per_lines+(TILE_HEIGHT*4))		

	ldi XL,lo8(sprites_per_lines) 
	ldi XH,hi8(sprites_per_lines)	


	;Outer loop used registers
	;-----------------------------
	;r9 = playfield resume tile row
	;r10 = total scanlines to draw
	;r14 = 1=overlay, 0=playfield rendering
	;r15 = resume scroll line
	;r20:21 = initial playfield row
	;r18 = overlay lines counter
	;r22 = current tile row
	;r23 = current scroll line
	;X = Sprites rendering line
	;Y = VRAM rendering position

m2_next_text_line:	
	;***draw line***
	rcall mode2_do_hsync_and_sprites 

	rjmp .
	rjmp .

	//Clear sprite buffer for next render line
	movw r6,XL //push

	ldi XL,lo8(sprite_buf_erase)
	ldi XH,hi8(sprite_buf_erase)
	
	ldi r16,TRANSLUCENT_COLOR
	ldi r17,4
spr_clr_loop:
	ld ZL,X+
	ld ZH,X+
	st Z+,r16
	st Z+,r16
	st Z+,r16
	st Z+,r16
	st Z+,r16
	st Z+,r16

	dec r17
	brne spr_clr_loop

	movw XL,r6


	dec r10
	breq m2_text_frame_end
	

	inc r23
	inc r22

	rjmp .

	cpi r22,8 ;last tile row? 1
	breq m2_next_text_row 
	
	;wait to align with next_tile_row instructions (+1 cycle for the breq)
	ldi r16,8
	dec r16
	brne .-4

	rjmp m2_next_text_line	

m2_next_text_row:
	clr r22		;current tile row			;1	
	
	adiw YL,VRAM_TILES_H ;32	;process next line in VRAM ;2

	;get Y scroll wrap adress
	in r0,_SFR_IO_ADDR(GPIOR1)
	in r1,_SFR_IO_ADDR(GPIOR2)

	lds r2,playfield_start_lo
	lds r3,playfield_start_hi

	inc r18

	brvc .+2   ;overlay finished?
	movw YL,r2 ;restore playfield adress
	
	brvc .+2     ;overlay finished?
	mov r22,r9   ;restore tile row

	brvc .+2     ;overlay finished?
	mov r23,r15  ;restore scroll row

	brvc .+2     ;overlay finished?
	clr r14		 ;put last since it clears the V flag
	
	clr r4
	cpi r23,0 	;wrap Y?
	cpc r14,r4
	brne .+2 	
	movw YL,r0	;load wrap adress
	
	rjmp m2_next_text_line

m2_text_frame_end:

	ldi r16,5
	dec r16
	brne .-4
	nop

	call hsync_pulse ;145
	

m2_text_end2:
	
	;set vsync flag if beginning of next frame
	ldi ZL,1
	sts vsync_flag,ZL

	;clear any pending timer int
	ldi ZL,(1<<OCF1A)
	sts _SFR_MEM_ADDR(TIFR1),ZL

	ret

;.rept 50
; 	fmulsu r20,r20
;.endr

;**************************
; Render sprite+H-Sync pulse+sound mix
; Cycles: 144
; Can destroy: r0,r1, r2,r3, r4,r5, r6,r7, r8, r11,r12 ,r13, r16,r17, r19,  r24,r25,Z
;**************************
mode2_do_hsync_and_sprites:
	;Important: TCNT1 should be equal to:
	;0x44 on the cbi 
	;0xcd on the sbi 
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;2
	
	call update_sound_buffer_fast ;29

	push r18
	movw r2,YL //save Y register
	
	;prepare some constants usedfor all aprites
	lds r12,sprites_tiletable_lo
	lds r13,sprites_tiletable_hi
	ldi r24,TILE_WIDTH*TILE_HEIGHT
	ldi r18,SPRITE_STRUCT_SIZE 	
	ldi r19,TILE_WIDTH
	clr r25

	;*** Process sprite #1 ***
	;address good sprite info line
	ld r16,X+		;get sprite # + y offset in sprite
	mov r17,r16
	andi r16,0x1f	;keep sprite#
	swap r17
	lsr r17
	andi r17,0x07	;keep y offset (3 msbits)

	ldi YL,lo8(sprites) 
	ldi YH,hi8(sprites)	
	mul r16,r18		;sprite no*sprite struct size
	add YL,r0		;index sprite in struct
	adc YH,r1

	ld r4,Y 	;get x pos
	movw ZL,r12 ;get sprites tile table base
	ldd r16,Y+2 ;SpriteTileNo
	mul r16,r24 ;tileheight*tilewidth
	add ZL,r0	;point to first sprite tile pixel in flash
	adc ZH,r1
	mul r17,r19	;get sprite tile line
	add ZL,r0
	adc ZH,r1
	
	ldi YL,lo8(scanline_sprite_buf)
	ldi YH,hi8(scanline_sprite_buf)
	
	add YL,r4
	adc YH,r25 ;zero

	sts sprite_buf_erase,YL		;save addr for buffer clear
	sts sprite_buf_erase+1,YH
	;30

	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0 
	;30

	;*** Process sprite #2 ***
	;address good sprite info line
	ld r16,X+	;get sprite # + y offset in sprite
	mov r17,r16
	andi r16,0x1f ;keep sprite#
	swap r17
	lsr r17
	andi r17,0x07 ;keep y offset (3 msbits)

	ldi YL,lo8(sprites) 
	ldi YH,hi8(sprites)	
	mul r16,r18 ;sprite no*sprite struct size
	add YL,r0
	adc YH,r1

	ld r4,Y 	;get x pos
	movw ZL,r12 ;get sprites tile table base
	ldd r16,Y+2 ;SpriteTileNo
	mul r16,r24 ;tileheight*tilewidth
	add ZL,r0	;point to first sprite tile pixel in flash
	adc ZH,r1
	mul r17,r19	;get sprite tile line
	add ZL,r0
	adc ZH,r1
	
	ldi YL,lo8(scanline_sprite_buf)
	ldi YH,hi8(scanline_sprite_buf)
	add YL,r4
	sts sprite_buf_erase+2,YL		;save addr for buffer clear
	adc YH,r25 ;zero
	;*** toggle sync pin  - EXACT TIMING REQD***
	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;2
	sts sprite_buf_erase+3,YH


	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0 
	;30


	;*** Process sprite #3 ***
	;address good sprite info line
	ld r16,X+	;get sprite # + y offset in sprite
	mov r17,r16
	andi r16,0x1f ;keep sprite#
	swap r17
	lsr r17
	andi r17,0x07 ;keep y offset (3 msbits)

	ldi YL,lo8(sprites) 
	ldi YH,hi8(sprites)	
	mul r16,r18 ;sprite no*sprite struct size
	add YL,r0
	adc YH,r1

	ld r4,Y 	;get x pos
	movw ZL,r12 ;get sprites tile table base
	ldd r16,Y+2 ;SpriteTileNo
	mul r16,r24 ;tileheight*tilewidth
	add ZL,r0	;point to first sprite tile pixel in flash
	adc ZH,r1
	mul r17,r19	;get sprite tile line
	add ZL,r0
	adc ZH,r1
	
	ldi YL,lo8(scanline_sprite_buf)
	ldi YH,hi8(scanline_sprite_buf)
	add YL,r4
	adc YH,r25 ;zero

	sts sprite_buf_erase+4,YL		;save addr for buffer clear
	sts sprite_buf_erase+5,YH
	;30

	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0 
	;30

	;*** Process sprite #4 ***
	;address good sprite info line
	ld r16,X+	;get sprite # + y offset in sprite
	mov r17,r16
	andi r16,0x1f ;keep sprite#
	swap r17
	lsr r17
	andi r17,0x07 ;keep y offset (3 msbits)

	ldi YL,lo8(sprites) 
	ldi YH,hi8(sprites)	
	mul r16,r18 ;sprite no*sprite struct size
	add YL,r0
	adc YH,r1

	ld r4,Y 	;get x pos
	movw ZL,r12 ;get sprites tile table base
	ldd r16,Y+2 ;SpriteTileNo
	mul r16,r24 ;tileheight*tilewidth
	add ZL,r0	;point to first sprite tile pixel in flash
	adc ZH,r1
	mul r17,r19	;get sprite tile line
	add ZL,r0
	adc ZH,r1
	
	ldi YL,lo8(scanline_sprite_buf)
	ldi YH,hi8(scanline_sprite_buf)
	add YL,r4
	adc YH,r25 ;zero

	sts sprite_buf_erase+6,YL		;save addr for buffer clear
	sts sprite_buf_erase+7,YH
	;30

	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0
	lpm r0,Z+
	st Y+,r0 
	;30

	movw YL,r2	;restore Y

	lds ZL,sync_pulse
	dec ZL
	sts sync_pulse,ZL

	;*************************************************
	; RENDER TILE LINE MODE 2
	; r22 = Current tile row (0-7)
	;*************************************************

	movw r6,XL	;save X

	movw XL,YL

	mov r19,YL
	andi r19,0xe0 ;mask for 32-bytes line wrap

	ldi r17,TRANSLUCENT_COLOR ;transparent color

	ldi YL,lo8(scanline_sprite_buf+6)
	ldi YH,hi8(scanline_sprite_buf+6)

	lds r4,tile_table_lo
	lds r5,tile_table_hi

	;add tile Y offset
	ldi r18,TILE_WIDTH
	mul r22,r18
	movw r24,r0

	;scolling branch (17 cycles)
	ldi ZL,lo8(mode2_render_offsets)	
	ldi ZH,hi8(mode2_render_offsets)

#if MODE2_HORIZONTAL_SCROLLING == 1	
	lds r8,scroll_x_offset	
	

	ldi r20,0
	cpse r14,r20
	clr r8			;prevent scrolling of the overlay
	
#else
	clr r8
	nop
	nop
	nop
	nop
#endif

	lsl r8 ;*2	
	clr r1
	add ZL,r8
	adc ZH,r1
	
	lpm r0,Z+
	lpm r1,Z
	movw ZL,r0

	lsr r8 ;/2 -> used in line functions

	;load the first tile from vram
	ld	r20,X	;load tile no from VRAM
	ldi r18,TILE_HEIGHT*TILE_WIDTH
	mul r20,r18 ;tile*width*height

	inc XL
	andi XL,0x1f
	or XL,r19	;restore higher bits

	add r0,r4	;add title table address
	adc r1,r5

	add r0,r24	;add tile Y offset
	adc r1,r25  ;add tile Y offset	

	ijmp	;call render line functions
	

;******* OFFSET 0 ********************
mode2_render_line_offset_0:
	;r1:r0 = Tile row index
	movw ZL,r0

	lpm
	lpm
	lpm
	lpm

	;first out must be at cycle 14
.rept 22
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16
	
	ld	r20,X //+	;load tile # from VRAM

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16

	mul r20,r18 ;tile*width*height
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16

	add r0,r4	;add title table address
	adc r1,r5
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw YL,r2 ;restore
	movw XL,r6 ;restore 
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15

#if MODE2_HORIZONTAL_SCROLLING == 1

;******* OFFSET 1 ********************
mode2_render_line_offset_1:
	;r1:r0 = Tile row index
	movw ZL,r0
	
	adiw ZL,1 ;add tile x-scroll offset

	ld r20,X 	;load next tile # from VRAM

	rjmp .
	rjmp .
	rjmp .
	rjmp .

	;first out must be at cycle 14
.rept 22
	
	lpm r16,Z+	 ;load bg pixel
	ld  r13,Y+   ;load sprite pixel
	cpse r13,r17 ;check transparency
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 2

	mul r20,r18 ;tile*width*height
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 3

	add r0,r4	;add title table address
	adc r1,r5
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 4

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 5

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 6

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 1
	
	ld	r20,X 	;load next tile # from VRAM
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw XL,r6
	movw YL,r2
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15



;******* OFFSET 2 ********************
mode2_render_line_offset_2:
	;r1:r0 = Tile row index
	movw ZL,r0
	
	adiw ZL,2 ;add tile x-scroll offset
	
	ld r20,X 	;load next tile # from VRAM
	mul r20,r18 ;tile*width*height
	
	rjmp .
	rjmp .
	rjmp .

	;first out must be at cycle 14
.rept 22
		
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 3

	add r0,r4	;add title table address
	adc r1,r5
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 4

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 5

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 6

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 1
	
	ld	r20,X 	;load next tile # from VRAM

	lpm r16,Z+	 ;load bg pixel
	ld  r13,Y+   ;load sprite pixel
	cpse r13,r17 ;check transparency
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 2

	mul r20,r18 ;tile*width*height
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw XL,r6
	movw YL,r2
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15


;******* OFFSET 3 ********************
mode2_render_line_offset_3:
	;r1:r0 = Tile row index
	movw ZL,r0
	
	adiw ZL,3 ;add tile x-scroll offset
	
	ld r20,X 	;load next tile # from VRAM
	mul r20,r18 ;tile*width*height
	add r0,r4	;add title table address
	adc r1,r5
	
	rjmp .
	rjmp .

	;first out must be at cycle 14
.rept 22
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 4

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 5

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 6

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 1
	
	ld	r20,X 	;load next tile # from VRAM

	lpm r16,Z+	 ;load bg pixel
	ld  r13,Y+   ;load sprite pixel
	cpse r13,r17 ;check transparency
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 2

	mul r20,r18 ;tile*width*height

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 3

	add r0,r4	;add title table address
	adc r1,r5
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw XL,r6
	movw YL,r2
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15



;******* OFFSET 4 ********************
mode2_render_line_offset_4:
	;r1:r0 = Tile row index
	movw ZL,r0
	
	adiw ZL,4 ;add tile x-scroll offset
	
	ld r20,X 	;load next tile # from VRAM
	mul r20,r18 ;tile*width*height
	add r0,r4	;add title table address
	adc r1,r5
	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 
	
	rjmp .

	;first out must be at cycle 14
.rept 22
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 5

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 6

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 1
	
	ld	r20,X 	;load next tile # from VRAM

	lpm r16,Z+	 ;load bg pixel
	ld  r13,Y+   ;load sprite pixel
	cpse r13,r17 ;check transparency
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 2

	mul r20,r18 ;tile*width*height

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 3

	add r0,r4	;add title table address
	adc r1,r5
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 4

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw XL,r6
	movw YL,r2
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15



;******* OFFSET 5 ********************
mode2_render_line_offset_5:
	;r1:r0 = Tile row index
	movw ZL,r0
	
	adiw ZL,5 ;add tile x-scroll offset
	
	ld r20,X 	;load next tile # from VRAM
	mul r20,r18 ;tile*width*height
	add r0,r4	;add title table address
	adc r1,r5
	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 
	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally	
	

	;first out must be at cycle 14
.rept 22
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 6

	or XL,r19	;restore higher bits
	movw ZL,r0	;load next tile adress

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 1
	
	ld	r20,X 	;load next tile # from VRAM

	lpm r16,Z+	 ;load bg pixel
	ld  r13,Y+   ;load sprite pixel
	cpse r13,r17 ;check transparency
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 2

	mul r20,r18 ;tile*width*height

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 3

	add r0,r4	;add title table address
	adc r1,r5
	
	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 4

	add r0,24	;add tile table row offset 
	adc r1,25 ;add tile table row offset 

	lpm r16,Z+
	ld  r13,Y+
	cpse r13,r17
	mov r16,r13
	out _SFR_IO_ADDR(DATA_PORT),r16 ;Tile Pixel 5

	inc XL		
	andi XL,0x1f	;wrap to 32 tiles horizontally
.endr

	;end set last pix to zero
	pop r18
	nop
	nop
	clr r16
	movw XL,r6
	movw YL,r2
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret ;15

#endif

#endif


.rept 50
 	fmulsu r20,r20
.endr
	

#if VIDEO_MODE == 1

;***************************************************
; Video Mode 1: Tiles-Only
; Process video frame in tile mode (40*28)
;***************************************************	

sub_video_mode1:

	;waste time to align with next hsync in render function ;1554
	ldi r24,lo8(390)
	ldi r25,hi8(390)
	sbiw r24,1
	brne .-4
	nop
	
	ldi YL,lo8(vram)
	ldi YH,hi8(vram)

	ldi r23,SCREEN_TILES_V*TILE_HEIGHT; total scanlines to draw (28*8)	
	clr r22 ;current tile line

next_tile_line:	
	rcall hsync_pulse 

	ldi r19,36 + CENTER_ADJUSTMENT
	dec r19			
	brne .-4

	;***Render scanline***
	call render_tile_line

	ldi r19,27 - CENTER_ADJUSTMENT
	dec r19			
	brne .-4
	nop

	dec r23
	breq text_frame_end
	
	lpm	;3 nop
	inc r22

	cpi r22,8 ;last line of a tile? 1
	breq next_tile_row 
	
	;wait to align with next_tile_row instructions (+1 cycle for the breq)
	lpm ;3 nop
	lpm ;3 nop
	lpm ;3 nop
	nop
	rjmp next_tile_line	

next_tile_row:
	clr r22		;current char line			;1	

	ldi r19,VRAM_TILES_H*2
	add YL,r19
	adc YH,r22

	lpm ;3 nop
	nop
	nop

	rjmp next_tile_line

text_frame_end:

	ldi r19,5
	dec r19
	brne .-4
	rjmp .

	rcall hsync_pulse ;145

	;set vsync flag if beginning of next frame (each two fields)
	ldi r17,1
	lds r16,curr_field
	eor r16,r17
	sts curr_field,r16
	sbrs r16,0
	sts vsync_flag,r17

	;clear any pending timer int
	ldi ZL,(1<<OCF1A)
	sts _SFR_MEM_ADDR(TIFR1),ZL

	ret

;*************************************************
; RENDER TILE LINE
;
; r22     = Y offset in tiles
; r23 	  = tile width in bytes
; Y       = VRAM adress to draw from (must not be modified)
;
; Must preserve: r22,r23,Y
; 
;*************************************************
render_tile_line:
	;save Y
	movw XL,YL

	;add tile Y offset
	ldi r16,TILE_WIDTH ;tile width in pixels
	mul r22,r16

	;load the first tile from vram
	ld	ZL,X+	;load tile adress LSB from VRAM
	ld	ZH,X+	;load tile adress MSB from VRAM
	add ZL,r0	;add tile address
	adc ZH,r1   ;add tile address	
		
	;draw 40 tiles wide, 6 clocks/pixel
	ldi r17,40

m1_rtl_loop:
	lpm r16,Z+	;load tile pixel 0
	out _SFR_IO_ADDR(DATA_PORT),r16
	ld	r20,X+	;load tile adress # LSB from VRAM
	lpm r16,Z+	;load tile pixel 1
	
	out _SFR_IO_ADDR(DATA_PORT),r16
	ld	r21,X+	;load tile adress # MSB from VRAM
	lpm r16,Z+	;load tile pixel 2
	
	out _SFR_IO_ADDR(DATA_PORT),r16
	add r20,r0	;add tile table row offset 	
	adc r21,r1	;add tile table row offset 	
	lpm r16,Z+	;load tile pixel 3
	
	out _SFR_IO_ADDR(DATA_PORT),r16
	rjmp .
	lpm r16,Z+	;load tile pixel 4
	
	out _SFR_IO_ADDR(DATA_PORT),r16
	lpm r16,Z+	;load tile pixel 5
	movw ZL,r20	;copy next tile adress 
	dec r17		;decrement tile counter
	
	out _SFR_IO_ADDR(DATA_PORT),r16
	brne m1_rtl_loop


	;end set last pix to zero
	lpm ;3 nops
	clr r16	
	out _SFR_IO_ADDR(DATA_PORT),r16

	ret
#endif


;************
; HSYNC
;************
do_hsync:
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ; HDRIVE sync pulse low

	call update_sound_buffer ;36 -> 63

	ldi ZL,32-9
do_hsync_delay:
	dec ZL
	brne do_hsync_delay ;135

	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;136

	rcall set_normal_rate_HDRIVE


	ldi ZL,SYNC_PHASE_PRE_EQ
	ldi ZH,SYNC_PRE_EQ_PULSES

	rcall update_sync_phase

	sbrs ZL,0
	rcall render

	sbrs ZL,0
	rjmp not_start_of_frame


	;call sound mixing if first vsync pulse
	call MixSound

not_start_of_frame:


	ret


;*****************************************
; READ JOYPADS
; read 60 time/sec before redrawing screen
;*****************************************

read_joypads:

	//latch data
	sbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_LATCH_PIN
	jmp . ; wait ~200ns
	jmp . ;(6 cycles)
	cbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_LATCH_PIN
	
	//clear button state bits
	clr r20 //P1
	clr r21

	clr r22 //P2
	clr r23

	ldi r24,12 //number of buttons to fetch
	
	jmp .
	jmp .
	jmp .
	jmp .
	jmp .
	jmp .
;29
	
read_joypads_loop:	
	;read data pin

	lsl r20
	rol r21
	sbic _SFR_IO_ADDR(JOYPAD_IN_PORT),JOYPAD_DATA1_PIN
	ori r20,1
	
	lsl r22
	rol r23
	sbic _SFR_IO_ADDR(JOYPAD_IN_PORT),JOYPAD_DATA2_PIN
	ori r22,1
	

	;pulse clock pin
	sbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_CLOCK_PIN
	jmp . ;wait 6 cycles
	jmp .
	cbi _SFR_IO_ADDR(JOYPAD_OUT_PORT),JOYPAD_CLOCK_PIN

	jmp .
	jmp .
	jmp .
	jmp .

	dec r24
	brne read_joypads_loop ;232

	com r20 
	com r21
	com r22
	com r23

#if JOYSTICK == TYPE_NES
	;Do some bit transposition
	bst r21,3
	bld r20,3
	bst r21,2
	bld r21,3
	andi r21,0b00001011
	andi r20,0b11111000

	bst r23,3
	bld r22,3
	bst r23,2
	bld r23,3
	andi r23,0b00001011
	andi r22,0b11111000

#elif JOYSTICK == TYPE_SNES
	andi r21,0b00001111
	andi r23,0b00001111
#endif 

	sts joypad1_status_lo,r20
	sts joypad1_status_hi,r21

	sts joypad2_status_lo,r22
	sts joypad2_status_hi,r23

	ret




;**** RENDER ****
render:
	push ZL

	lds ZL,sync_pulse
	cpi ZL,SYNC_HSYNC_PULSES-FIRST_RENDER_LINE
	brsh render_end
	cpi ZL,SYNC_HSYNC_PULSES-FIRST_RENDER_LINE-(SCREEN_TILES_V*TILE_HEIGHT)
	brlo render_end
	
	push r2
	push r3
	push r4
	push r5

	push r6
	push r7
	push r8
	push r9

	push r10
	push r11
	push r12
	push r13

	push r14
	push r15
	push r16
	push r17

	push r18
	push r19
	push r20
	push r21

	push r22
	push r23
	push r24
	push r25

	push XL
	push XH
	push YL
	push YH 
		
	
	#if VIDEO_MODE == 1
		call sub_video_mode1
	#else
		call sub_video_mode2
	#endif


	pop YH
	pop YL
	pop XH
	pop XL

	pop r25
	pop r24
	pop r23
	pop r22

	pop r21
	pop r20
	pop r19
	pop r18

	pop r17
	pop r16
	pop r15
	pop r14

	pop r13
	pop r12
	pop r11
	pop r10

	pop r9
	pop r8
	pop r7
	pop r6

	pop r5
	pop r4
	pop r3
	pop r2

render_end:
	pop ZL
	ret

;***************************************************************************
; Video sync interrupt
; 4 cycles to invoke 
;
;***************************************************************************
TIMER1_COMPA_vect:
	push ZH;2
	push ZL;2

	;save flags & status register
	in ZL,_SFR_IO_ADDR(SREG);1
	push ZL ;2		

	;Read timer offset since rollover to remove cycles 
	;and conpensate for interrupt latency.
	;This is nessesary to eliminate frame jitter.

	lds ZL,_SFR_MEM_ADDR(TCNT1L)
	subi ZL,0x0e ;MIN_INT_LATENCY

	cpi ZL,1
	brlo .		;advance PC to next instruction

	cpi ZL,2
	brlo .		;advance PC to next instruction

	cpi ZL,3
	brlo .		;advance PC to next instruction

 	cpi ZL,4
	brlo .		;advance PC to next instruction

	cpi ZL,5
	brlo .		;advance PC to next instruction

	cpi ZL,6
	brlo .		;advance PC to next instruction

	cpi ZL,7
	brlo .		;advance PC to next instruction
	
	cpi ZL,8
	brlo .		;advance PC to next instruction

	cpi ZL,9
	brlo .		;advance PC to next instruction

	rcall sync
	;call update_sound_buffer

	pop ZL
	out _SFR_IO_ADDR(SREG),ZL
	pop ZL
	pop ZH
	reti


;***************************************************
; Composite SYNC
;***************************************************

sync:
	push r0
	push r1
			
	ldi ZL,lo8(sync_func_vectors)	
	ldi ZH,hi8(sync_func_vectors)

	lds r0,sync_phase
	lsl r0 ;*2
	clr r1
	
	add ZL,r0
	adc ZH,r1
	
	lpm r0,Z+
	lpm r1,Z
	movw ZL,r0

	icall	;call sync functions

	pop r1
	pop r0
	ret


;*************************************************
; Generate a H-Sync pulse - 136 clocks (4.749us)
; Note: TCNT1 should be equal to 
; 0x44 on the cbi 
; 0xcc on the sbi 
;
; Cycles: 144
; Destroys: ZL (r30)
;*************************************************
hsync_pulse:
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;2
	
	call update_sound_buffer ;36 -> 63
	
	ldi ZL,30-9
wait_hsync_p:
	dec ZL 
	brne wait_hsync_p ;92



	lds ZL,sync_pulse
	dec ZL
	sts sync_pulse,ZL

	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;2

	nop
	nop

	ret


;**************************
; PRE_EQ pulse
; Note: TCNT1 should be equal to 
; 0x44 on the cbi
; 0x88 on the sbi
; pulse duration: 68 clocks
;**************************
do_pre_eq:
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ; HDRIVE sync pulse low

	call update_sound_buffer_2 ;36 -> 63

	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;68
	nop

	ldi ZL,SYNC_PHASE_EQ
	ldi ZH,SYNC_EQ_PULSES
	rcall update_sync_phase

	rcall set_double_rate_HDRIVE

	ret

;************
; Serration EQ
; Note: TCNT1 should be equal to 
; 0x44  on the cbi
; 0x34A on the sbi
; low pulse duration: 774 clocks
;************
do_eq:
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ; HDRIVE sync pulse low

	call update_sound_buffer_2 ;36 -> 63

	ldi ZL,181-9+4
do_eq_delay:
	nop
	dec ZL
	brne do_eq_delay ;135

	nop
	nop

	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;136

	ldi ZH,SYNC_POST_EQ_PULSES
	ldi ZL,SYNC_PHASE_POST_EQ
	rcall update_sync_phase
		
	ret

;************
; POST_EQ
; Note: TCNT1 should be equal to 
; 0x44 on the cbi
; 0x88 on the sbi
; pulse cycles: 68 clocks
;************
do_post_eq:
	cbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ; HDRIVE sync pulse low

	call update_sound_buffer_2 ;36 -> 63

	sbi _SFR_IO_ADDR(SYNC_PORT),SYNC_PIN ;68

	nop

	ldi ZL,SYNC_PHASE_HSYNC
	ldi ZH,SYNC_HSYNC_PULSES
	rcall update_sync_phase

	ret






;************
; update sync phase
; ZL=next phase #
; ZH=next phases's pulse count
;
; returns: ZL: 0=more pulses in phase
;              1=was the last pulse in phase
;***********
update_sync_phase:

	lds r0,sync_pulse
	dec r0
	lds r1,_SFR_MEM_ADDR(SREG)
	sbrc r1,SREG_Z
	mov r0,ZH
	sts sync_pulse,r0	;set pulses for next sync phase

	lds r0,sync_phase
	sbrc r1,SREG_Z
	mov r0,ZL			;go to next phase 
	sts sync_phase,r0
	
	ldi ZL,0
	sbrc r1,SREG_Z
	ldi ZL,1

	ret

;**************************************
; Set HDRIVE to double rate during VSYNC
;**************************************
set_double_rate_HDRIVE:

	ldi ZL,hi8(HDRIVE_CL_TWICE)
	sts _SFR_MEM_ADDR(OCR1AH),ZL
	
	ldi ZL,lo8(HDRIVE_CL_TWICE)
	sts _SFR_MEM_ADDR(OCR1AL),ZL

	ret

;**************************************
; Set HDRIVE to normal rate
;**************************************
set_normal_rate_HDRIVE:

	ldi ZL,hi8(HDRIVE_CL)
	sts _SFR_MEM_ADDR(OCR1AH),ZL
	
	ldi ZL,lo8(HDRIVE_CL)
	sts _SFR_MEM_ADDR(OCR1AL),ZL

	ret


/*******************************************************************
 *******************************************************************
 * Memory size dependent functions
 *******************************************************************
 *******************************************************************/

#if VRAM_ADDR_SIZE == 2
	;***********************************
	; CLEAR VRAM
	; Fill the screen with the specified tile
	; C-callable
	;************************************
	ClearVram:
		//init vram		
		ldi r30,lo8(VRAM_TILES_H*VRAM_TILES_V)
		ldi r31,hi8(VRAM_TILES_H*VRAM_TILES_V)

		ldi XL,lo8(vram)
		ldi XH,hi8(vram)


		lds r22,font_table_lo
		lds r23,font_table_hi

	fill_vram_loop:

		st X+,r22
		st X+,r23	
		sbiw r30,1
		brne fill_vram_loop

		clr r1

		ret

	;***********************************
	; LOAD Main map
	;************************************
	LoadMap:
		push r16
		push r17
		//init vram
	
		ldi r24,lo8(VRAM_TILES_H *VRAM_TILES_V)
		ldi r25,hi8(VRAM_TILES_H *VRAM_TILES_V)
		ldi XL,lo8(vram)
		ldi XH,hi8(vram)

		lds ZL,tile_map_lo
		lds ZH,tile_map_hi

		ldi r20,(TILE_WIDTH*TILE_HEIGHT) ;48

		lds r16,tile_table_lo
		lds r17,tile_table_hi

	load_map_loop:
		lpm r22,Z+ ;16
		lpm r23,Z+ ;17

		mul r22,r20
		movw r18,r0
		mul r23,r20
		add r19,r0

		add r18,r16
		adc r19,r17

		st X+,r18	;store tile adress
		st X+,r19

		sbiw r24,1
		brne load_map_loop

		clr r1

		pop r17
		pop r16
		ret

	;***********************************
	; RESTORE TILE
	; Copy a map tile # to the same position VRAM
	; C-callable
	; r24=X pos (8 bit)
	; r22=Y pos (8 bit)
	;************************************
	RestoreTile:
		clr r25
		clr r23
		clr r19
		ldi r18,VRAM_TILES_H*2
		mul r22,r18		;calculate Y line addr
		lsl r24
		add r0,r24		;add X offset
		adc r1,r19

		//load map tile #
		lds ZL,tile_map_lo
		lds ZH,tile_map_hi

		add ZL,r0
		adc ZH,r1
		lpm r20,Z+ 
		lpm r21,Z+ 

		ldi XL,lo8(vram)
		ldi XH,hi8(vram)
		add XL,r0
		adc XH,r1

		ldi r18,(TILE_WIDTH*TILE_HEIGHT)
		mul r20,r18
		movw r22,r0
		mul r21,r18
		add r23,r0

		lds r20,tile_table_lo
		lds r21,tile_table_hi

		add r22,r20
		adc r23,r21

		st X+,r22
		st X,r23

		clr r1

		ret

	;***********************************
	; SET FONT TILE
	; C-callable
	; r24=X pos (8 bit)
	; r22=Y pos (8 bit)
	; r20=Font tile No (8 bit)
	;************************************
	SetFont:
		clr r25
		clr r21

		ldi r18,VRAM_TILES_H
		lsl r18	

		mul r22,r18		;calculate Y line addr in vram
		lsl r24
		add r0,r24		;add X offset
		adc r1,r25
		ldi XL,lo8(vram)
		ldi XH,hi8(vram)
		add XL,r0
		adc XH,r1

		lds r22,font_table_lo
		lds r23,font_table_hi

		ldi r18,(TILE_WIDTH*TILE_HEIGHT)
		mul r20,r18
		add r22,r0
		adc r23,r1

		st X+,r22
		st X,r23

		clr r1
	
		ret


	;***********************************
	; SET TILE
	; C-callable
	; r24=X pos (8 bit)
	; r22=Y pos (8 bit)
	; r21:r20=Tile No (16 bit)
	;************************************
	SetTile:
		clr r25
		clr r23	

		;ldi r18,40*2
		ldi r18,VRAM_TILES_H
		lsl r18	

		mul r22,r18		;calculate Y line addr in vram
		lsl r24
		add r0,r24		;add X offset
		adc r1,r25
		ldi XL,lo8(vram)
		ldi XH,hi8(vram)
		add XL,r0
		adc XH,r1

		lds r22,tile_table_lo
		lds r23,tile_table_hi

		ldi r18,(TILE_WIDTH*TILE_HEIGHT)
		mul r20,r18
		add r22,r0
		adc r23,r1

		mul r21,r18
		add r23,r0

		st X+,r22
		st X,r23

		clr r1
	
		ret

#endif

#if VRAM_ADDR_SIZE == 1
	;***********************************
	; CLEAR VRAM 8bit
	; Fill the screen with the specified tile
	; C-callable
	;************************************
	ClearVram:
		//init vram		
		ldi r30,lo8(VRAM_TILES_H*(VRAM_TILES_V+OVERLAY_HEIGHT))
		ldi r31,hi8(VRAM_TILES_H*(VRAM_TILES_V+OVERLAY_HEIGHT))

		ldi XL,lo8(vram)
		ldi XH,hi8(vram)

		clr r22

	fill_vram_loop:
		st X+,r22
		sbiw r30,1
		brne fill_vram_loop

		clr r1

		ret

		
	;***********************************
	; SET TILE 8bit mode
	; C-callable
	; r24=X pos (8 bit)
	; r22=Y pos (8 bit)
	; r20=Tile No (8 bit)
	;************************************
	SetTile:

		clr r25
		clr r23	

		ldi r18,VRAM_TILES_H

		mul r22,r18		;calculate Y line addr in vram
		add r0,r24		;add X offset
		adc r1,r25
		ldi XL,lo8(vram)
		ldi XH,hi8(vram)
		add XL,r0
		adc XH,r1

		st X,r20

		clr r1
	
		ret

	;***********************************
	; Load the "Main Map" 8bit : unsupported
	;************************************
	LoadMap:
		ret

	;***********************************
	; SET FONT TILE
	; C-callable
	; r24=X pos (8 bit)
	; r22=Y pos (8 bit)
	; r20=Font tile No (8 bit)
	;************************************
	SetFont:
		clr r25

		ldi r18,VRAM_TILES_H

		mul r22,r18		;calculate Y line addr in vram
		
		add r0,r24		;add X offset
		adc r1,r25

		ldi XL,lo8(vram)
		ldi XH,hi8(vram)
		add XL,r0
		adc XH,r1

		lds r21,font_tile_index
		add r20,r21

		st X,r20

		clr r1
	
		ret


	;***********************************
	; SET FONT Index
	; C-callable
	; r24=First font tile index in tile table (8 bit)
	;************************************
 	SetFontTilesIndex:
		sts font_tile_index,r24
		ret

#endif





;****************************************************
; INITIALIZATION
;****************************************************
InitVideo:

	;setup timer 1 to generate interrupts on 
	;twice the line rate.
	
	cli

	;stop timers
	clr r20
	sts _SFR_MEM_ADDR(TCCR1B),r20
	sts _SFR_MEM_ADDR(TCCR0B),r20
	
	//*** Set params *****

	;set port directions
	ldi r20,0xff
	out _SFR_IO_ADDR(DDRC),r20 ;video dac
	out _SFR_IO_ADDR(DDRB),r20 ;h-sync for ad725

	ldi r20,0x80
	out _SFR_IO_ADDR(DDRD),r20 ;audio-out, midi-in

	;setup port A for joypads
	/*
	;setup port A for joypads
	ldi r20,_SFR_IO_ADDR(DDRA) 
	andi r20,0b11111100 ;data in 
	ori  r20,0b00001100 ;control lines
	out _SFR_IO_ADDR(DDRA),r20
	cbi _SFR_IO_ADDR(PORTA),PA2
	cbi _SFR_IO_ADDR(PORTA),PA3
	*/

	ldi r20, 0b00001100	;  only control lines are outputs
	out _SFR_IO_ADDR(DDRA),r20
	;ldi r20, 0b11110011 ; pullups on the data lines
	//out _SFR_IO_ADDR(PORTA),r20 
	
#if MIDI_IN == ENABLED
	;set UART for MIDI in
	//ldi r20,(1<<RXCIE0)+(1<<RXEN0)
	ldi r20,(1<<RXEN0)
	sts _SFR_MEM_ADDR(UCSR0B),20

	ldi r20,(1<<UCSZ01)+(1<<UCSZ00)
	sts _SFR_MEM_ADDR(UCSR0C),20

	ldi r20,56 ;31250 bauds (.5% error)
	sts _SFR_MEM_ADDR(UBRR0L),r20
#endif

	;set sync parameters
	;starts at odd field, in pre-eq pulses, line 1
	
	ldi r16,SYNC_PHASE_PRE_EQ
	sts sync_phase,r16
	ldi r16,SYNC_PRE_EQ_PULSES
	sts sync_pulse,r16


	;clear counters
	sts _SFR_MEM_ADDR(TCNT1H),r20
	sts _SFR_MEM_ADDR(TCNT1L),r20	


	;set sync generator counter on TIMER1
	ldi r20,hi8(HDRIVE_CL_TWICE)
	sts _SFR_MEM_ADDR(OCR1AH),r20
	
	ldi r20,lo8(HDRIVE_CL_TWICE)
	sts _SFR_MEM_ADDR(OCR1AL),r20


	ldi r20,(1<<WGM12)+(1<<CS10) 
	sts _SFR_MEM_ADDR(TCCR1B),r20;CTC mode, use OCR1A for match

	ldi r20,(1<<OCIE1A)
	sts _SFR_MEM_ADDR(TIMSK1),r20 ;generate interrupt on match



	;set clock divider counter for AD725 on TIMER0
	;outputs 14.31818Mhz (4FSC)
	ldi r20,(1<<COM0A0)+(1<<WGM01)  ;toggle on compare match + CTC
	sts _SFR_MEM_ADDR(TCCR0A),r20

	ldi r20,0
	sts _SFR_MEM_ADDR(OCR0A),r20	;divide main clock by 2

	ldi r20,(1<<CS00)  ;enable timer, no pre-scaler
	nop
	sts _SFR_MEM_ADDR(TCCR0B),r20


	;set sound PWM on TIMER2
	ldi r20,(1<<COM2A1)+(1<<WGM21)+(1<<WGM20)   ;Fast PWM	
	sts _SFR_MEM_ADDR(TCCR2A),r20

	ldi r20,0
	sts _SFR_MEM_ADDR(OCR2A),r20	;duty cycle (amplitude)

	ldi r20,(1<<CS20)  ;enable timer, no pre-scaler
	sts _SFR_MEM_ADDR(TCCR2B),r20

	ldi r20,(1<<SYNC_PIN)|(1<<VIDEOCE_PIN)
	out _SFR_IO_ADDR(SYNC_PORT),r20	;set sync & chip enable line to hi

	
	//ldi r20,VRAM_TILES_H
	//sts tiles_per_line,r20

	sei

	clr r1

	#if VIDEO_MODE == 2
		//clear sprite-scanline buffer
		ldi r20,(SCREEN_TILES_H+2)*TILE_WIDTH
		ldi XL,lo8(scanline_sprite_buf)
		ldi XH,hi8(scanline_sprite_buf)
		ldi r16,TRANSLUCENT_COLOR
		
	clr_ss:
		st X+,r16
		dec r20
		brne clr_ss

		ldi r20,0
		sts overlay,r20
	#endif

	ret




;***********************************
; Define the tile data source
; C-callable
; r25:r24=pointer to font tiles data
;************************************
SetFontTable:
	sts font_table_lo,r24
	sts font_table_hi,r25
	
	ret

;***********************************
; Define the tile data source
; C-callable
; r25:r24=pointer to tiles data
;************************************
SetTileTable:
	sts tile_table_lo,r24
	sts tile_table_hi,r25
	
	ret

;*****************************
; Defines a tile map
; C-callable
; r25:r24=pointer to tiles map
;*****************************
SetTileMap:
	//adiw r24,2
	sts tile_map_lo,r24
	sts tile_map_hi,r25

	ret


;************************************
; This flag is set on each VSYNC by
; the engine. This func is used to
; synchronize the programs on frame
; rate (30hz).
;
; C-callable
;************************************
GetVsyncFlag:
	lds r24,vsync_flag
	ret

;*****************************
; Clear the VSYNC flag.
; 
; C-callable
;*****************************
ClearVsyncFlag:
	clr r1
	sts vsync_flag,r1
	ret

;*****************************
; Return joypad 1 or 2 buttons status
; C-callable
; r24=joypad No (0 or 1)
; returns: (int) r25:r24
;*****************************
ReadJoypad_int:	
	tst r24
	brne rj_p2
		
	lds r24,joypad1_status_lo
	lds r25,joypad1_status_hi
	ret
rj_p2:
	lds r24,joypad2_status_lo
	lds r25,joypad2_status_hi	

	;check for reset condition


	ret

;*****************************
; Sets CPU speed to 14.3Mhz
;*****************************
SetLowSpeed:
	ldi r24,0x80
	ldi r25,1
	sts _SFR_MEM_ADDR(CLKPR),r24
	sts _SFR_MEM_ADDR(CLKPR),r25
	ret

;*****************************
; Sets CPU speed to normal 
;*****************************
SetFullSpeed:
	ldi r24,0x80
	ldi r25,0
	sts _SFR_MEM_ADDR(CLKPR),r24
	sts _SFR_MEM_ADDR(CLKPR),r25
	ret


#if VIDEO_MODE == 2

;*****************************
; Defines where the sprites tile are defined.
; C-callable
; r25:r24=pointer to sprites pixel data.
;*****************************
SetSpritesTileTable:
	sts sprites_tiletable_lo,r24
	sts sprites_tiletable_hi,r25
	ret

	
;***********************************
; Sets the overlay parameters
; C-callable
; r24= b7	 Priority: 0=overlay is below sprites, 1=overlay above sprites
;	   b6    Location: 0=top, 1=bottom (not implemented yet)
;      b5:b4 Reserved
;      b3:b0 Height in tiles (0-15). 0=overlay off
;************************************
SetOverlay:
	sts overlay,r24
	ret

#endif 
