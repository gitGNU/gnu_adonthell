/*
   $Id$

   Copyright (C) 2000 Andrew Henderson <hendersa@db.erau.edu>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "audio.h"

// #include "audio_loop.h"

bool audio::audio_initialized = false;

int audio::background_volume;
int audio::effects_volume;
#ifdef OGG_MUSIC
// loop_info *audio::loop[NUM_MUSIC];
#endif
Mix_Music *audio::music[NUM_MUSIC];
string audio::music_file[NUM_MUSIC];
Mix_Chunk *audio::sounds[NUM_WAVES];
bool audio::background_on = false;
int audio::current_background;
bool audio::background_paused;
int audio::audio_rate;
Uint16 audio::buffer_size;
Uint16 audio::audio_format;
int audio::audio_channels;

// python schedule stuff
py_object audio::schedule;
bool audio::schedule_active = 0;
PyObject *audio::schedule_args = NULL;

// callback to get notified when music finished playing
void on_music_finished ()
{
    audio::set_background_finished (true);
}

void audio::init (config *myconfig) {
  int i;  // Generic counter variable

  // Sample rate: 11025, 22050 or 44100 Hz
  switch( myconfig->audio_sample_rate ) {
    case 0: {
      audio_rate = 11025;
      break; }
    case 1: {
      audio_rate = 22050;
      break; }
    default: {
      audio_rate = 44100;
      break; }
  }
  
  // Output in signed 8/16-bit form
  audio_format = myconfig->audio_resolution == 0 ? AUDIO_S8 : AUDIO_S16; 

  // 1 is mono, 2 is stereo
  audio_channels = myconfig->audio_channels == 0 ? 1 : 2;  

  // 100... scales to percentages ;>
  background_volume = myconfig->audio_volume;   

  buffer_size = 4096;        // Audio buffer size
  effects_volume = 128;	     // Still figuring this one out...
  background_on = false;     // No background music is playing
  current_background = -1;   // No song currently playing
  background_paused = false; // Music isn't paused
  audio_initialized = false; // No audio connection yet
  schedule_active = false;   // No schedule file yet
  
  // Mark all slots in sound and music arrays as empty
  for (i = 0; i < NUM_WAVES; i++) sounds[i] = NULL;
  for (i = 0; i < NUM_MUSIC; i++) {
      music[i] = NULL; 
      music_file[i] = "";
  }
  
  // Try opening the audio device at our defaults
  i = Mix_OpenAudio(audio_rate, audio_format, audio_channels, buffer_size);

  // Now see what we got when opening the audio device
  // If we COULDN'T open the audio...
  if ( i < 0 ) {
    fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    fprintf(stderr, "Audio will not be used.\n");

  // If we COULD open the audio...
  } else {
    audio_initialized = true;
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    fprintf(stderr, "Audio started in %d Hz %d bit %s format.\n", audio_rate,
     (audio_format&0xFF), (audio_channels > 1) ? "stereo" : "mono");
    set_background_volume (background_volume);
    
    // connect music finish callback
    Mix_HookMusicFinished (&on_music_finished);
  }
}

void audio::cleanup(void) {
  int i;
  background_on = false;    // no music is playing
  current_background = -1;  // No music is queued to play

  // Null out those tunes and sound effects
  for (i = 0; i < NUM_WAVES; i++) {
    unload_wave(i);
    sounds[i] = NULL;
  }
  for (i = 0; i < NUM_MUSIC; i++) {
    unload_background(i);
    music[i] = NULL;
    music_file[i] = "";
  }

  schedule.clear ();
  
  // Close out audio connection
  Mix_CloseAudio();
  audio_initialized = false;
}

int audio::load_background(int slot, char *filename) {

  if (!audio_initialized) return(1);

  // Check for bad input
  if ((slot >= NUM_MUSIC) || (slot < 0)) {
    fprintf(stderr, "Error: Tried to put music in invalid slot.\n");
    return(1);
  }

  //  Check if the file exists at all...
  FILE *f = fopen (filename, "r");
  if (!f)  
  {
      music[slot] = NULL; 
      return 1;
  }
  fclose (f);   

  // If we are loading background music into the slot
  // the current background music is in...
  if (current_background == slot) {

    // This warns the audio thread to stop updating
    background_on = false;
    current_background = -1;

    Mix_HaltMusic();  // Just a precaution
    unload_background(slot);

  // If we aren't loading background music over
  // music currently playing ...
  } else {

    // Music already occupies that slot
    if (music[slot] != NULL)
      unload_background(slot);
  }
  
  // No music in slot, load new tune in, ...
  music[slot] = Mix_LoadMUS(filename);
  music_file[slot] = filename;
  
#ifdef OGG_MUSIC
  // read loop points and ...
  // loop[slot] = new loop_info (&music[slot]->ogg_data.ogg->vf);
  
  // ... enable looping
  // music[slot]->ogg_data.ogg->vf.callbacks.read_func = &ogg_read_callback;
#endif
  return(0);
}

void audio::unload_background(int slot) {
  if (music[slot] != NULL) {
    Mix_FreeMusic(music[slot]);
    music[slot] = NULL;
    music_file[slot] = "";
    
    if (current_background == slot)
        current_background = -1;
        background_on = false;
#ifdef OGG_MUSIC
    // delete loop[slot];
#endif
  }
}

void audio::pause_music(void) {
  Mix_PauseMusic();
}

void audio::unpause_music(void) {
  Mix_ResumeMusic();
}

// Accepts a percentage of the maximum volume level
// and clips out of bounds values to 0-100.
void audio::set_background_volume(int volume) {

  // Check for bad input
  if (volume < 0) {
    background_volume = 0;
  } else if (volume > 100) {
    background_volume = 100;
  } else
    background_volume = volume;

  // Scales 0-100% to 0-128
  Mix_VolumeMusic(int(background_volume * 1.28));
}

// This should be done better, but I'll wait until
// I have enough sound effects to play with ;>
int audio::load_wave(int slot, char *filename) {

  if (!audio_initialized) return(1);
 
  // Check for bad input
  if ((slot >= NUM_WAVES) || (slot < 0)) {
    fprintf(stderr, "Error: Tried to put wave in invalid slot.\n");
    return(1);
  } else {
      //  Check if the file exists at all...
      FILE *f = fopen (filename, "r");
      if (!f)  
      {
          sounds[slot] = NULL; 
          return 1;
      }
      
      fclose (f);   
      
      sounds[slot] = Mix_LoadWAV(filename);
  }
  return(0);
}

void audio::unload_wave(int wave) {
  if (sounds[wave] != NULL) {
    Mix_FreeChunk(sounds[wave]);
    sounds[wave] = NULL;
  }
}

void audio::play_wave(int channel, int slot) {
  if ((slot > -1) && (slot < NUM_CHANNELS))
    if (sounds[slot] != NULL) Mix_PlayChannel(channel, sounds[slot], 0);
}

void audio::play_background(int slot) {
  if (music[slot] != NULL) {
    current_background = slot;
    background_on = true;
    Mix_PlayMusic(music[current_background], 0);
  }
}

void audio::fade_out_background(int time) {
  if (background_on == true) {
    Mix_FadeOutMusic(time);
#ifdef OGG_MUSIC
    // music[current_background]->ogg_data.ogg->vf.callbacks.read_func = &fread_wrap;
#endif
    background_on = false;
    current_background = -1;
  }
}

void audio::fade_in_background(int slot, int time) {
  if ((background_on == false) && (music[slot] != NULL)) {
    current_background = slot;
    background_on = true;
    Mix_FadeInMusic(music[slot], 0, time);
  }
}

// Temporary convience function for testing
void audio::change_background(int slot, int time) {
  fade_out_background(time);
  fade_in_background(slot, time);
}

#ifdef OGG_MUSIC
// OggVorbis_File* audio::get_vorbisfile ()
// {
//     return &music[current_background]->ogg_data.ogg->vf;
// }
#endif

// set audio schedule
void audio::set_schedule (string file, PyObject * args)
{     
    // Clears the schedule
    schedule.clear ();
    Py_XDECREF (schedule_args);
    schedule_args = NULL;

    // Set new schedule
    if (file != "")
    {
        schedule_args = args; 
        Py_XINCREF (schedule_args); 
        schedule.create_instance ("schedules/audio/" + file, file, args);
    }
}

// run the audio control schedule
void audio::run_schedule ()
{
    PyObject *song = Py_BuildValue ("(i)", current_background);
    if (schedule_active) schedule.call_method ("music_finished", song);
    Py_DECREF (song);
}

// save state
s_int8 audio::put_state (ogzstream& file)
{
    // avilable music 
    NUM_MUSIC >> file;
    for (int i = 0; i < NUM_MUSIC; i++) music_file[i] >> file;
    
    // currently playing
    current_background >> file;
    
    // Save the schedule script state
    schedule.object_file () >> file;
    if (schedule_args) 
    {
        true >> file; 
        python::put_tuple (schedule_args, file);
    }
    else false >> file; 
    is_schedule_activated () >> file;
    
    return 1;
}

// get state
s_int8 audio::get_state (igzstream& file)
{
    string tune, script;
    int num_music;
    bool have_args;
    
    // avilable music 
    num_music << file;
    for (int i = 0; i < NUM_MUSIC && i < num_music; i++)
    {
        tune << file;
        if (tune != "") load_background (i, (char*) tune.c_str ());
    }

    // current background
    num_music << file;
        
    // Restore the schedule script state
    PyObject * args = NULL; 
    script << file;
    
    have_args << file;
    if (have_args) args = python::get_tuple (file); 
    set_schedule (script, args);      
    Py_XDECREF (args); 
    
    schedule_active << file;

    // start music
    if (num_music != -1) play_background (num_music);

    return 1;
}
