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
#include <SDL/SDL.h>
#include "fileops.h"
#include "types.h"
#include "mapevent.h"
#include "mapcharacter.h"
#include "map.h"
#include "mapengine.h"

void mapevent::get(SDL_RWops * file)
{
  u_int16 i;
  SDL_RWread(file,&type,sizeof(type),1);
  for(i=1;i<10;i++)
    SDL_RWread(file,&param[i],sizeof(param[i]),1);
  getstringfromfile(param_str,file);
  SDL_RWread(file,&otherevent,sizeof(otherevent),1);
}

void mapevent::put(SDL_RWops * file)
{
  u_int16 i;
  SDL_RWwrite(file,&type,sizeof(type),1);
  for(i=1;i<10;i++)
    SDL_RWwrite(file,&param[i],sizeof(param[i]),1);
  putstringtofile(param_str,file);
  SDL_RWwrite(file,&otherevent,sizeof(otherevent),1);
}

void mapevent::map_action_0(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
}

void mapevent::map_action_1(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  aguy->set_movtype(0);
  if((param[7])&&(aguy->get_nbr()==255)) mapengine::fade_out(amap);
  amap->leave_character(aguy->get_posx(),aguy->get_posy());
  aguy->set_posx(param[1]);
  aguy->set_posy(param[2]);
  if(param[3]!=255) aguy->set_framefactor(param[3]);
  amap->put_character(aguy->get_posx(),aguy->get_posy(),aguy);
  aguy->init_moveframe();
  if(aguy->get_nbr()==255)
    {
      amap->set_posx(param[4]);
      amap->set_posy(param[5]);
      if(param[6]!=255) amap->set_scrolltype(param[6]);
      amap->set_movtype(0);
    }
  if((param[7])&&(aguy->get_nbr()==255)) mapengine::fade_in(amap);
}


void mapevent::map_action_2(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (amap->get_scrolltype()==1) amap->set_scrolltype(2);
      else if (amap->get_scrolltype()==2) amap->set_scrolltype(1);
    }
}

void mapevent::map_action_3(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(1);
}

void mapevent::map_action_4(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_scrolltype(2);
}

void mapevent::map_action_5(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param[7]) mapengine::fade_out(amap);
      amap->~landmap();
      amap->load(param_str);
      amap->init_for_scrolling();
      if (param[7]) mapengine::fade_in(amap);
    }
}

void mapevent::map_action_6(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255)
    {
      if (param[7]) mapengine::fade_out(amap);
      amap->leave_character(aguy->get_posx(),aguy->get_posy());
      amap->~landmap();
      amap->load(param_str);
      aguy->set_posx(param[1]);
      aguy->set_posy(param[2]);
      if(param[3]!=255) aguy->set_framefactor(param[3]);
      amap->set_posx(param[4]);
      amap->set_posy(param[5]);
      if(param[6]!=255) amap->set_scrolltype(param[6]);
      amap->init_for_scrolling();
      if(param[7]) mapengine::fade_in(amap);
    }
}

void mapevent::map_action_7(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
//      Enable Horizontal Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param[1])
	{
	  if(aguy->get_movtype()!=param[1])
	      return;
	}
      amap->enable_horizontal_scrolling();
    }
}

void mapevent::map_action_8(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
//      Disable Horizontal Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param[1])
	{
	  if(aguy->get_movtype()!=param[1])
	      return;
	}
      amap->disable_horizontal_scrolling();
    }
}

void mapevent::map_action_9(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
//      Enable Vertical Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param[1])
	{
	  if(aguy->get_movtype()!=param[1])
	      return;
	}
      amap->enable_vertical_scrolling();
    }
}

void mapevent::map_action_10(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
//     Disable Vertical Scrolling
{
  if (aguy->get_nbr()==255)
    {
      if (param[1])
	{
	  if(aguy->get_movtype()!=param[1])
	      return;
	}
      amap->disable_vertical_scrolling();
    }
}

void mapevent::map_action_11(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  landmap tmap;
  if(aguy->get_nbr()!=255) return;
  aguy->set_movtype(0);
  mapengine::fade_out(amap);
  tmap.load(param_str);
  // Launch mapengine on tmap
  //  update_mapkeyboard(amap);
  mapengine::fade_in(amap);
}

void mapevent::map_action_255(mapcharacter*aguy,landmap *amap,u_int16 x,u_int16 y)
{
  if (aguy->get_nbr()==255) amap->set_status(1);
}

u_int8 mapevent::action (mapcharacter * aguy, landmap * amap, u_int16 x, u_int16 y)
{
  switch(type)
    {
    case 0: map_action_0(aguy,amap,x,y); break;
    case 1: map_action_1(aguy,amap,x,y); break;
    case 2: map_action_2(aguy,amap,x,y); break;
    case 3: map_action_3(aguy,amap,x,y); break;
    case 4: map_action_4(aguy,amap,x,y); break;
    case 5: map_action_5(aguy,amap,x,y); break;
    case 6: map_action_6(aguy,amap,x,y); break;
    case 7: map_action_7(aguy,amap,x,y); break;
    case 8: map_action_8(aguy,amap,x,y); break;
    case 9: map_action_9(aguy,amap,x,y); break;
    case 10: map_action_10(aguy,amap,x,y); break;
    case 11: map_action_11(aguy,amap,x,y); break;
    case 255: map_action_255(aguy,amap,x,y); break;
    }
  return(otherevent);
}
