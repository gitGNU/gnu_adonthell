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
#include "mapcharacter.h"
#include "mappattern.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "window.h"
#include "map.h"
#include "prefs.h"
#include "mapengine.h"
#ifdef SDL_MIXER
#include "audio_thread.h"
#include "audio.h"
#endif

void mapengine::map_engine(landmap * amap)
{
  amap->set_status(0);
  amap->init_for_scrolling();
  //  amap->win.init(70,20,180,60,WIN_NORMAL);
  //  amap->win.set_action(TEXT_WRITE);
  //  amap->win.show();
#ifdef SDL_MIXER
  audio_in->fade_in_background(1,500);
#endif
  fade_in(amap);
  while(1)
    {
      update_and_show(amap);
      if (amap->get_status()) break;
      screen::show();
    }
#ifdef SDL_MIXER
  audio_in->fade_out_background(500);
#endif
  fade_out(amap);
}

void mapengine::update_and_show(landmap * amap)
{
  static u_int16 i;

  for(i=0;i<screen::frames_to_do;i++)
    {
      //dlg->update_keyboard ();
      amap->update_keyboard();
      amap->update_patterns();
      amap->update_all_characters();
      //    amap->win.update();
    }
  //dlg->update(amap->win);
  amap->update_status();
  amap->draw_down();
  amap->draw_all_characters();
  amap->draw_up();

  //  amap->win.draw();
}

void mapengine::fade_out(landmap * amap, u_int16 depx=56, u_int16 depy=12) 
{
  s_int8 i;
  amap->set_status(MAP_STATUS_FADE);
  for (i=0;i<90;i+=screen::frames_to_do*2) 
    {
      update_and_show(amap); 
      screen::makesquare(depx,depy,i);
      screen::show();
    }
  amap->set_status(MAP_STATUS_NORMAL);
}

void mapengine::fade_in(landmap * amap, u_int16 depx=56, u_int16 depy=12) 
{
  s_int8 i;
  amap->set_status(MAP_STATUS_FADE);
  for (i=90;i>0;i-=screen::frames_to_do*2) 
    {
      update_and_show(amap); 
      screen::makesquare(depx,depy,i);
      screen::show();
    }
  amap->set_status(MAP_STATUS_NORMAL);
}
