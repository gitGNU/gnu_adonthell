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

u_int8 input::charkeys_pushed[65536];
u_int8 input::charkeys_released[65536];
bool input::charkeys[65536];
u_int16 input::last_key;

u_int16 input::mouse_posx, input::mouse_posy;
bool input::mouse_button[3];

void input::update() 
{
  SDL_Event event;
  SDL_EnableUNICODE(1);
  // All key events are saved in the "charkeys" variable in the
  // keyboard namespace.
  while(SDL_PollEvent(&event))
    {
      switch (event.type) {
      case SDL_KEYDOWN:
	input::charkeys_pushed[event.key.keysym.sym]++;
	input::charkeys[event.key.keysym.sym]=true;
	input::last_key = event.key.keysym.sym;
	break;
      case SDL_KEYUP:
	input::charkeys_released[event.key.keysym.sym]++;
	input::charkeys[event.key.keysym.sym]=false;
	break;
      case SDL_MOUSEMOTION:
	input::mouse_posx=event.motion.x;
	input::mouse_posy=event.motion.y;
	break;
      case SDL_MOUSEBUTTONDOWN:
	input::mouse_button[event.button.button-1]=true;
	break;
      case SDL_MOUSEBUTTONUP:
	input::mouse_button[event.button.button-1]=false;
	break;
      }
    }
}

u_int16 input::getkeypressed() { return(last_key); }


bool input::is_pushed(u_int16 key)
{
  return(charkeys[key]);
}

u_int8 input::get_nbr_pushed(u_int16 k)
{
  static u_int8 r;
  r=charkeys_pushed[k];
  charkeys_pushed[k]=0;
  return r;
}

u_int8 input::get_nbr_released(u_int16 k)
{
  static u_int8 r;
  r=charkeys_released[k];
  charkeys_released[k]=0;
  return r;
}
