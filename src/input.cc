/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "image.h"
#include "input.h"
#include "SDL.h"

//u_int8 input::keyboard_mode;
u_int8 * input::keystate=NULL;
u_int8 * input::p_keystate=NULL;
u_int16 input::last_key;
s_int32 input::keystatelength;
//bool is_unicode_on=false;

u_int16 input::mouse_posx, input::mouse_posy;
bool input::mouse_button[3];


int input::FilterEvents(const SDL_Event *event)
{
  if(event->type==SDL_KEYDOWN) p_keystate[event->key.keysym.sym]++;
  return 1;
}

void input::init()
{
  //  keyboard_mode=0;
  keystate=SDL_GetKeyState(&keystatelength);
  //  set_keyboard_mode(MODE_STATE);
  p_keystate=new u_int8[keystatelength];
  memset(p_keystate, 0, keystatelength);
  set_key_repeat(0,0);
  SDL_SetEventFilter(FilterEvents);
  SDL_EnableUNICODE(1);
}

void input::shutdown()
{
  delete[] p_keystate;
}

void input::update()
{
  SDL_PumpEvents();
}

bool input::is_pushed(SDLKey key)
{
  bool ret;
  ret=keystate[key];
  if((ret)&&(p_keystate[key])) p_keystate[key]--;
  return ret;
}

bool input::has_been_pushed(SDLKey key)
{
  bool ret;
  ret=p_keystate[key];
  if((ret)&&(!(--p_keystate[key]))) keystate[key]=0;
  return ret;
}

void input::set_key_repeat(int delay=SDL_DEFAULT_REPEAT_DELAY, int interval=SDL_DEFAULT_REPEAT_INTERVAL)
{
  SDL_EnableKeyRepeat(delay, interval);
}

s_int32 input::get_next_key()
{
  static SDL_Event event;
  static bool b;
  b=false;
  if(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_KEYDOWNMASK)==1)
    {
      b=true;
      if(p_keystate[event.key.keysym.sym]) p_keystate[event.key.keysym.sym]--;
      keystate[event.key.keysym.sym]=0;
    }
  // FIXME: this should be placed elsewhere.
  while(SDL_PeepEvents
	(&event,1,SDL_GETEVENT,SDL_ALLEVENTS-SDL_KEYDOWNMASK)==1);
  if (b) return(event.key.keysym.sym);
  return(-1);
}

s_int32 input::get_next_unicode()
{
  static SDL_Event event;
  static bool b;
  b=false;
  if(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_KEYDOWNMASK)==1)
    {
      b=true;
      if(event.key.keysym.unicode)
	{
	  if(p_keystate[event.key.keysym.sym]) p_keystate[event.key.keysym.sym]--;
	  keystate[event.key.keysym.sym]=0;
	}
    }
  // FIXME: this should be placed elsewhere.
  while(SDL_PeepEvents
	(&event,1,SDL_GETEVENT,SDL_ALLEVENTS-SDL_KEYDOWNMASK)==1);
  if (b) return(event.key.keysym.unicode);
  return(-1);
}

void input::clear_keys_queue()
{
  while(get_next_key()!=-1);
  memset(p_keystate, 0, keystatelength);
}
