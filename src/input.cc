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
#include "gfx.h"
#include "input.h"
#include "SDL.h"

bool input::charkeys[65536];
u_int16 input::last_key;

u_int16 input::mouse_posx, input::mouse_posy;
bool input::mouse_button[3];

int input_update(void *) {

  SDL_Event event;
  SDL_EnableUNICODE(1);

  // All key events are saved in the "charkeys" variable in the
  // keyboard namespace.
  while ( SDL_WaitEvent(&event) ) {
    switch (event.type) {
    case SDL_KEYDOWN:
      input::charkeys[event.key.keysym.sym]=1;
      input::last_key = event.key.keysym.sym;
      break;
    case SDL_KEYUP:
      input::charkeys[event.key.keysym.sym]=0;
      break;
    case SDL_MOUSEMOTION:
      input::mouse_posx=event.motion.x;
      input::mouse_posy=event.motion.y;
      break;
    case SDL_MOUSEBUTTONDOWN:
      input::mouse_button[event.button.button]=true;
      break;
    case SDL_MOUSEBUTTONUP:
      input::mouse_button[event.button.button]=false;
      break;
    }
  }
  return(0);
}

u_int16 input::getkeypressed() { return(last_key); }


bool input::is_pushed(u_int16 key)
{
  return(charkeys[key]);
}
