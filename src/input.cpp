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

u_int8 input::keyboard_mode;
u_int8 * input::keystate=NULL;
u_int8 * input::p_keystate=NULL;
u_int16 input::last_key;

u_int16 input::mouse_posx, input::mouse_posy;
bool input::mouse_button[3];


int input::FilterEvents(const SDL_Event *event)
{
  if(event->type==SDL_KEYDOWN) p_keystate[event->key.keysym.sym]++;
  return 1;
}

void input::init()
{
  int n;
  keyboard_mode=0;
  keystate=SDL_GetKeyState(&n);
  set_keyboard_mode(MODE_STATE);
  set_key_repeat(0,0);
  SDL_SetEventFilter(FilterEvents);
  p_keystate=new u_int8[n];
  memset(p_keystate, 0, n);
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
  if(keyboard_mode==MODE_PUSHED) keystate[key]=0;
  if(keyboard_mode==MODE_PUSHED) ret=p_keystate[key]; else ret=keystate[key];
  if(p_keystate[key]) p_keystate[key]--;
  return ret;
}

void input::set_keyboard_mode(u_int8 mode)
{
  if((mode==MODE_CHAR)&&(keyboard_mode!=MODE_CHAR))
    {
      SDL_EnableUNICODE(1);
      set_key_repeat();
      //      SDL_SetEventFilter(NULL);
    }
  if((mode==MODE_STATE)&&(keyboard_mode!=MODE_STATE))
    {
      SDL_EnableUNICODE(0);
      set_key_repeat(0,0);
      //      SDL_SetEventFilter(NULL);
    }
  if((mode==MODE_PUSHED)&&(keyboard_mode!=MODE_PUSHED))
    {
      SDL_EnableUNICODE(0);
      set_key_repeat(0,0);
    }
  keyboard_mode=mode;
}

void input::set_key_repeat(int delay=SDL_DEFAULT_REPEAT_DELAY, int interval=SDL_DEFAULT_REPEAT_INTERVAL)
{
  SDL_EnableKeyRepeat(delay, interval);
}

u_int8 input::get_keyboard_mode()
{
  return(keyboard_mode);
}

s_int32 input::get_next_key()
{
  static SDL_Event event;
  if(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_KEYDOWNMASK)==1)
    {
      if(p_keystate[event.key.keysym.sym]) p_keystate[event.key.keysym.sym]--;
      if(keyboard_mode==MODE_PUSHED) keystate[event.key.keysym.sym]=0;
      if(keyboard_mode==MODE_CHAR) return(event.key.keysym.unicode);
      else if((keyboard_mode==MODE_STATE)||(keyboard_mode==MODE_PUSHED)) 
	return(event.key.keysym.sym);
      else return(0);
    }
  else return(-1);
}

void input::clear_keys_queue()
{
  cout << "Clearing\n";
  while(get_next_key()!=-1);
  cout << "Done it\n";
}
