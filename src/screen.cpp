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

#include <iostream>
#include <stdlib.h>
#include "screen.h"

using namespace std;

u_int16 screen::l;
u_int16 screen::h;
u_int32 screen::SDL_flags=0;
u_int8 screen::ftd;
u_int8 screen::bytes_per_pixel;
u_int32 screen::trans;
u_int32 screen::trans_rgb;
SDL_Surface * screen::vis;
u_int32 screen::timer1, screen::timer2;
bool screen::cur_visible=false;

// Decrease this to speed up the game
const u_int32 screen::CYCLE_LENGTH = 13;


void screen::set_video_mode(u_int16 nl, u_int16 nh)
{
  u_int8 bpp;
  SDL_flags |= SDL_HWSURFACE;
  SDL_flags |= SDL_DOUBLEBUF;

  if (SDL_Init (SDL_INIT_VIDEO) < 0) {
    fprintf (stderr, "couldn't init SDL: %s\n", SDL_GetError ());
    exit (1);
  }
  
  bpp = SDL_VideoModeOK(nl, nh, 16, SDL_flags);

  switch (bpp)
    {
    case 0:
      fprintf(stderr, "Video mode %dx%d unavailable. Exiting.. \n",nl, nh);
      exit (1);
      break;
    case 16:
      printf ("Using 16bpp depth: %dx%d.\n",nl,nh);
      bytes_per_pixel = 2;
      break;
    case 24:
      printf ("Using 24bpp depth: %dx%d.\n",nl,nh);
      bytes_per_pixel = 3;
      break;
    default:
      printf ("Emulating 16bpp depth in %dbpp mode: %dx%d.\n",bpp,nl,nh);
      bpp=16;
      bytes_per_pixel = 2;
      break;
    }
  l=nl;
  h=nh;

  // Set the video mode
  vis=SDL_SetVideoMode(nl, nh, bpp, SDL_HWSURFACE | SDL_flags);
  if (vis == NULL) 
    {
      fprintf (stderr, "error: %s\n", SDL_GetError ());
      exit (1);
    }

  // Turning off mouse cursor
  set_mouse_cursor_visible(false);

  // Setting up the window title
  SDL_WM_SetCaption ("Adonthell", NULL);
  
  // Setting up transparency colors
  trans=SDL_MapRGB(vis->format,0xFF,0x00,0xFF);
  trans_rgb=0xFF00FF;

  // Global initialisations
  ftd=1;
  init_frame_counter();
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
  vis=SDL_SetVideoMode(l,h,bytes_per_pixel*8,SDL_flags);
}

bool screen::get_fullscreen()
{
  return(SDL_flags&SDL_FULLSCREEN);
}

void screen::init_frame_counter()
{
  timer1=SDL_GetTicks();
}

void screen::show()
{
    // Syncronize the game's speed to the machine it's running on
    while (1)
    {
        timer2 = SDL_GetTicks() - timer1;

        if (timer2 >= CYCLE_LENGTH) break;
        else if (timer2 > 3) SDL_Delay (timer2 - 2);
    }

    timer1 = SDL_GetTicks() - (timer2 % CYCLE_LENGTH);

    SDL_Flip (vis);

    // How slow is our machine? :)
    ftd = timer2 / CYCLE_LENGTH;
    if (ftd > FTD_LIMIT) ftd = FTD_LIMIT;
}

void screen::drawbox(u_int16 x, u_int16 y, u_int16 w, u_int16 h, 
		     u_int32 color, drawing_area * da_opt=NULL)
{
  static SDL_Rect dr;
  if(da_opt)
    {
      drawing_area da_tmp(x,y,w,h);
      da_tmp.assign_drawing_area(da_opt);
      dr=da_tmp.get_rects();
    }
  else
    {
      dr.x=x;
      dr.y=y;
      dr.w=w;
      dr.h=h;
    }
  SDL_FillRect(vis, &dr, color);
}

void screen::clear()
{
  drawbox(0,0,l,h,0);
}

/*

*/

void screen::makesquare(u_int16 px,u_int16 py, u_int16 fact)
{
  drawbox(0,py,l,fact,0);
  drawbox(0,h-fact-py,l,fact,0);
  drawbox(px,0,fact,h,0);
  drawbox(l-fact-px,0,fact,h,0);
}

void screen::set_mouse_cursor_visible(bool b)
{
  if(b)
    {
      cur_visible=true;
      SDL_ShowCursor(1);
    }
  else
    {
      cur_visible=false;
      SDL_ShowCursor(0);
    }
}
