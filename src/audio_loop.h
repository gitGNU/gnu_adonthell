/*
   $Id$

   Copyright (C) 2001 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef __AUDIO_LOOP_H__
#define __AUDIO_LOOP_H__

#ifdef OGG_VORBIS
#include "vorbis/vorbisfile.h"
#endif

// Class containing the information necessary for looping .ogg files
class loop_info
{
public:
    loop_info ();
    bool load (char*);

    u_int32 start_page_pcm;
    u_int32 start_page_raw;
    u_int32 start;
    u_int32 end;
};

#ifdef OGG_VORBIS
// Callback passed to OggVorbis to read/and loop our background music
extern "C" size_t ogg_read_callback (void *, size_t, size_t, void*);

// We've got to access internal SDL_mixer data, so here's it's redefiniton
// from SDL_mixer/music_ogg.h:
typedef struct {
	int playing;
	int volume;
	OggVorbis_File vf;
	int section;
	SDL_AudioCVT cvt;
	int len_available;
	Uint8 *snd_available;
} OGG_music;

// from SDL_mixer/music.c:
struct _Mix_Music {
	enum {
		MUS_CMD,
		MUS_WAV,
		MUS_MOD,
		MUS_MID,
		MUS_OGG,
		MUS_MP3
	} type;
	union {
		OGG_music *ogg;
	} data;
	Mix_Fading fading;
	int fade_volume;
	int fade_step;
	int fade_steps;
	int error;
};
#endif

#endif // __AUDIO_LOOP_H__
