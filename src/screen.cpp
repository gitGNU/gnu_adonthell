// $Id$
/*
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
#include "screen.h"
#include "image.h"
#include "pnm.h"
#include "prefs.h"
#include "input.h"

u_int16 screen::width;
u_int16 screen::height;
u_int32 screen::SDL_flags=0;
u_int8 screen::frames_to_do;
u_int8 screen::bytes_per_pixel;
u_int32 screen::trans;

SDL_Surface * screen::vis;

void screen::set_video_mode(u_int16 w, u_int16 h, config * myconfig=NULL)
{
  u_int8 bpp;
  SDL_flags |= SDL_HWSURFACE;
  SDL_flags |= SDL_DOUBLEBUF;
  if(myconfig&&(myconfig->screen_mode == 1)) SDL_flags |= SDL_FULLSCREEN;

  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf (stderr, "couldn't init SDL: %s\n", SDL_GetError ());
    exit (1);
  }
  
  atexit (SDL_Quit);
  bpp = SDL_VideoModeOK(w, h, 16, SDL_flags);

  switch (bpp)
    {
    case 0:
      fprintf(stderr, "Video mode %dx%d unavailable. Exiting.. \n",w, h);
      exit (1);
      break;
    case 16:
      printf ("Using 16bpp depth: %dx%d.\n",w,h);
      bytes_per_pixel = 2;
      trans=0xF81F;  
      break;
    case 24:
      printf ("Using 24bpp depth: %dx%d.\n",w,h);
      bytes_per_pixel = 3;
      trans=0xFF00FF;  
      break;
    default:
      printf ("Emulating 16bpp depth in %dbpp mode: %dx%d.\n",bpp,w,h);
      bpp=16;
      bytes_per_pixel = 2;
      trans=0xF81F;
      break;
    }

  width=w;
  height=h;

  // Set the video mode
  vis=SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE | SDL_flags);
  if (vis == NULL) 
    {
      fprintf (stderr, "error: %s\n", SDL_GetError ());
      exit (1);
    }

  // Turning off mouse cursor
  SDL_ShowCursor(0);
  
  SDL_WM_SetCaption ("Adonthell", NULL);
  
  frames_to_do=1;
}

void screen::set_fullscreen(bool mode)
{
  if(mode)
    {
      SDL_flags|=SDL_FULLSCREEN;
    }
  else
    {
      SDL_flags-=SDL_FULLSCREEN;
    }
  vis=SDL_SetVideoMode(width,height,bytes_per_pixel*8,SDL_flags);
}

bool screen::get_fullscreen()
{
  return(SDL_flags&SDL_FULLSCREEN);
}

void screen::init_display(config *myconfig)
{
  set_video_mode(320,200,myconfig);
}

void screen::show()
{
  // Lenght of a game cycle (milliseconds). Decreasing it will increase 
  // the game's speed.
  const u_int32 cycle_length=13;
  static Uint32 timer1, timer2, timer3;

  // Waiting for being sync with a game cycle
  while(1)
    {
      timer2=SDL_GetTicks();
      timer3=timer2-timer1;
      if(timer3>=cycle_length) break;
      else if(timer3>3) SDL_Delay(timer3-2);
    }
  timer1=SDL_GetTicks();
  timer1-=(timer3%cycle_length);
  SDL_Flip(vis);
  //  SDL_UpdateRect (vis, 0, 0, 0, 0);
  // How slow is our machine? :)
  frames_to_do=timer3/cycle_length;
  if(frames_to_do>20) frames_to_do=20;
  if ((SDL_GetModState()&KMOD_ALT)&&(input::is_pushed(SDLK_RETURN)))
    {
      if(screen::get_fullscreen())
	screen::set_fullscreen(false);
      else screen::set_fullscreen(true);
      input::clear_keys_queue();
    }
}

void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, u_int32 color)
{
  static SDL_Rect dr;
  dr.x=x;
  dr.y=y;
  dr.w=w;
  dr.h=h;
  SDL_FillRect(vis, &dr, color);
}


void screen::makesquare(u_int16 px,u_int16 py, u_int16 fact)
{
  drawbox(0,py,320,fact,0);
  drawbox(0,200-fact-py,320,fact,0);
  drawbox(px,0,fact,200,0);
  drawbox(320-fact-px,0,fact,200,0);
}

void screen::mouse_cursor_off()
{
  SDL_ShowCursor(0);
}

void screen::mouse_cursor_on()
{
  SDL_ShowCursor(1);
}

u_int16 screen::get_width()
{
  return width;
}

u_int16 screen::get_height()
{
  return height;
}
