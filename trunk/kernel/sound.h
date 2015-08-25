/*
 *  Uzebox Kernel Sound engine defines
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

#ifndef __SOUNDENGINE_H_
#define __SOUNDENGINE_H_

#include <stdbool.h>

//IMPORTANT: To enable midi in code genration throught the firmware, define a 
//MIDI_IN_ENABLED option in the GCC preprocessor. 
//i.e: -DMIDI_IN_ENABLED in Project Options->Custom Options


#define WAVE_CHANNELS 3
#define NOISE_CHANNELS 1
#define CHANNELS WAVE_CHANNELS+NOISE_CHANNELS
#define SWEEP_UP   0x80
#define SWEEP_DOWN 0x00

/*
//Patch commands
#define PC_ENV_SPEED	0
#define PC_NOISE_PARAMS	1
#define PC_WAVE			2
#define PC_NOTE_UP		3
#define PC_NOTE_DOWN	4
#define PC_NOTE_CUT		5
#define PC_NOTE_HOLD 	6
#define PC_ENV_VOL		7
#define PC_PITCH		8
#define PC_TREMOLO_LEVEL	9
#define PC_TREMOLO_RATE	10

#define PATCH_END		0xff
*/



/*************************************
 * structs used by the assembler mixer
 * actual allocation is in soundEngineCore.s
 *************************************/



struct MixerWaveChannelStruct
{
	unsigned char volume;				//(0-255)
	unsigned int  step;					//8:8 fixed point
	unsigned char positionFrac; 		//8bit fractional part 
	unsigned int  position;				//16bit sample pointer	
};


struct MixerNoiseChannelStruct
{
	unsigned char volume;				//(0-255)
	unsigned char params;				//bit0=7/15 bits lfsr, b1:6=divider (samples between shifts)
	unsigned int  barrel;				//16bit LFSR barrel shifter
	unsigned char divider;				//divider accumulator
	unsigned char reserved1;
};

//Common type for both channels
struct MixerChannelStruct
{
	unsigned char volume;				//(0-255)
	unsigned const char structAlignment[5];	//dont access!
};

struct SubChannelsStruct
{
	struct MixerWaveChannelStruct wave[WAVE_CHANNELS];
	struct MixerNoiseChannelStruct noise;
};

struct MixerStruct
{
	union Channels{
		struct MixerChannelStruct all[CHANNELS];
		struct SubChannelsStruct type;	
	}channels;

};

extern void SetMixerNoiseParams(unsigned char params);
extern void SetMixerWave(unsigned char channel,unsigned char patch);
extern void SetMixerPitch(unsigned char channel,unsigned int step);
extern void SetMixerNote(unsigned char channel,unsigned char note);//,int volume);
extern void SetMixerVolume(unsigned char channel,unsigned char volume);
extern struct MixerStruct mixer;

/*************************************
 * structs used by the music player
 *************************************/

typedef void (*PatchCommand)(unsigned char channel, char value);

struct TrackStruct
{
	bool enabled;
	unsigned char priority;			//0=lowest
	unsigned char note;

	unsigned char tremoloPos;
	unsigned char tremoloLevel;
	unsigned char tremoloRate;

	unsigned char trackVol;
	unsigned char noteVol;
	unsigned char envelopeVol;		//(0-255)
	char envelopeStep;				//signed, amount of envelope change each frame +127/-128
	bool patchPlaying;
	unsigned char patchNo;
	unsigned char fxPatchNo;
	unsigned char patchLastStatus;
	unsigned char patchNextDeltaTime;
	unsigned char patchCurrDeltaTime;
	unsigned char patchPlayingTime;	//used by fx to steal oldest voice
	unsigned char patchWave;		//0-7
	bool patchEnvelopeHold;
	const char *patchCommandStreamPos;

};

/*
extern void TriggerNote(unsigned char channel,unsigned char patch,unsigned char note,unsigned char volume);
//extern void TriggerFx(unsigned char patch,unsigned char volume, bool retrig);
extern void TriggerFx(unsigned char patch,unsigned char volume, bool retrig);
extern void ProcessMusic(void);
extern void StopSong();
extern void StartSong(const char *midiSong);
extern void ResumeSong();
extern void InitMusicPlayer(const char *patchPointers[]);
*/


#endif
