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
#include "mapcharacter.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "mappattern.h"
#include "window.h"
#include "map.h"

mapcharacter::mapcharacter()
{
  posx=0;
  posy=0;
  addx=0;
  addy=0;
  prevmovtype=0;
  speeddelay=0;
  speedcounter=0;
  movtype=0;
  framefactor=0;
  walkcounter=0;
  moveframe=0;
  scridx=0;
  waitcounter=0;
  nbr_of_frames=0;
  is_event=0;
  mapcharnbr=0;
}

mapcharacter::~mapcharacter()
{
  u_int16 i;
  for (i=0;i<nbr_of_frames;i++)
    frame[i].~image();
  free(frame);
  frame=NULL;
  nbr_of_frames=0;
}

s_int8 mapcharacter::get(SDL_RWops * file)
{
  u_int16 i;
  s_int8 res;
  walkcounter=11;
  SDL_RWread(file,&nbr_of_frames,sizeof(nbr_of_frames),1);
  frame=new image[nbr_of_frames];
  for (i=0;i<nbr_of_frames;i++)
    {
      if ((res=frame[i].get(file))) return res;
      frame[i].set_mask(true);
      frame[i].set_trans(0);
    }
  return 0;
}

s_int8 mapcharacter::load(char * fname)
{
  u_int16 retvalue;
  SDL_RWops * file;
  file=SDL_RWFromFile(fname,"r");
  if(!file) return(1);
  else
    {
      retvalue=get(file);
      SDL_RWclose(file);
      return(retvalue);
    }
}

void mapcharacter::get_heroe_stat(SDL_RWops * file)
{
  SDL_RWread(file,&posx,sizeof(posx),1);
  SDL_RWread(file,&posy,sizeof(posy),1);
  SDL_RWread(file,&speeddelay,sizeof(speeddelay),1);
  SDL_RWread(file,&framefactor,sizeof(framefactor),1);
  mapcharnbr=255;
}

void mapcharacter::put_heroe_stat(SDL_RWops * file)
{
  SDL_RWwrite(file,&posx,sizeof(posx),1);
  SDL_RWwrite(file,&posy,sizeof(posy),1);
  SDL_RWwrite(file,&speeddelay,sizeof(speeddelay),1);
  SDL_RWwrite(file,&framefactor,sizeof(framefactor),1);
}

void mapcharacter::get_NPC_stat(SDL_RWops * file, u_int16 nbr)
{
  SDL_RWread(file,&posx,sizeof(posx),1);
  SDL_RWread(file,&posy,sizeof(posy),1);
  SDL_RWread(file,&speeddelay,sizeof(speeddelay),1);
  SDL_RWread(file,&framefactor,sizeof(framefactor),1);
  SDL_RWread(file,&movtype,sizeof(movtype),1);
  mapcharnbr=nbr;
}

void mapcharacter::put_NPC_stat(SDL_RWops * file)
{
  SDL_RWwrite(file,&posx,sizeof(posx),1);
  SDL_RWwrite(file,&posy,sizeof(posy),1);
  SDL_RWwrite(file,&speeddelay,sizeof(speeddelay),1);
  SDL_RWwrite(file,&framefactor,sizeof(framefactor),1);
  SDL_RWwrite(file,&movtype,sizeof(movtype),1);
}

u_int16 mapcharacter::mapcharacter::get_nbr()
{
  return(mapcharnbr);
}

u_int16 mapcharacter::mapcharacter::get_posx()
{
  return(posx);
}

u_int16 mapcharacter::mapcharacter::get_posy()
{
  return(posy);
}

u_int8 mapcharacter::get_addx()
{
  return(addx);
}

u_int8 mapcharacter::get_addy()
{
  return(addy);
}

u_int8 mapcharacter::get_scridx()
{
  return(scridx);
}

u_int8 mapcharacter::get_movtype()
{
  return(movtype);
}

u_int8 mapcharacter::get_speeddelay()
{
  return(speeddelay);
}

void mapcharacter::update_frame()
{
  if ((movtype!=0)&&(movtype!=5)) walkcounter++;
  if (walkcounter==(16*speeddelay))
    {
      if (moveframe==0) moveframe=1;
      else moveframe=0;
      walkcounter=0;
    }
  currentframe=framefactor+moveframe;
}

void mapcharacter::init_moveframe()
{
  moveframe=0;
  if ((posx%2)||(posy%2)) moveframe=1;
  if ((posx%2)&&(posy%2)) moveframe=0;
}

void mapcharacter::draw(u_int16 x, u_int16 y,drawing_area * da_opt)
{
  frame[currentframe].draw(x,y,da_opt);
}

/*void mapcharacter::draw_part(u_int16 x, u_int16 y, u_int16 bw, u_int16 bh, 
	       u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_mask_part(x,y,bw,bh,xo,yo);
}
*/
bool mapcharacter::is_ready()
{
  return(speedcounter==0);
}

bool mapcharacter::is_speed_ready()
{
  if (speedcounter==speeddelay)
    {
      speedcounter=0;
      return 1;
    }else return 0;
}

void mapcharacter::set_speeddelay(u_int8 sd)
{
  speeddelay=sd;
}

void mapcharacter::set_movtype(u_int8 mt)
{
  movtype=mt;
}

void mapcharacter::set_framefactor(u_int8 ff)
{
  framefactor=ff;
}

void mapcharacter::set_posx(u_int16 x)
{
  posx=x;
}

void mapcharacter::set_posy(u_int16 y)
{
  posy=y;
}

void mapcharacter::update_NPC(landmap * amap)
{
  if ((scridx==0)&&(is_ready())) 
    {
      update_dir();
      update_move(amap);
    }
  update_eventleave(amap);
  update_frame();
  update_pos(amap);
  update_eventcome(amap);
}

void mapcharacter::update_dir()
{
  switch (movtype)
    {
    case RIGHT:
      prevmovtype=LEFT;
      break;
    case LEFT:
      prevmovtype=RIGHT;
      break;
    case UP:
      prevmovtype=DOWN;
      break;
    case DOWN:
      prevmovtype=UP;
      break;
    case 0:
      prevmovtype=0;
      waitcounter++;
      if (waitcounter==(16*speeddelay)) waitcounter=0;
      break;
    }
  if (movtype!=5) do {movtype=rand()%5;} while(movtype==prevmovtype);
  if ((prevmovtype==0)&&(waitcounter!=0)) movtype=0;
}

void mapcharacter::update_move(landmap *amap)
{
  switch (movtype)
    {
    case DOWN:
      if (posy==amap->get_height()-1)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx,posy+1)) {movtype=0;break;}
	  if ((amap->is_occuped(posx,posy+1))||
	      (amap->is_solid_down(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx,posy+1,this);
	  framefactor=0;
	}
      break;
    case UP:
      if (posy==0)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx,posy-1)) {movtype=0;break;}
	  if ((amap->is_occuped(posx,posy-1))||
	      (amap->is_solid_up(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx,posy-1,this);
	  framefactor=2;
	}
      break;
    case RIGHT:
      if (posx==amap->get_lenght()-1)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx+1,posy)) {movtype=0;break;}
	  if ((amap->is_occuped(posx+1,posy))||
	      (amap->is_solid_right(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx+1,posy,this);
	  framefactor=4;
	}
      break;
    case LEFT:
      if (posx==0)
	{movtype=0;break;}
      else
	{
	  if (amap->is_others_unreachable(posx-1,posy)) {movtype=0;break;}
	  if ((amap->is_occuped(posx-1,posy))||
	      (amap->is_solid_left(posx,posy)))
	    {movtype=0;break;}
	  else amap->put_character(posx-1,posy,this);
	  framefactor=6;
	}
      break;
    }
}

void mapcharacter::update_main_move(landmap *amap)
{
  if(get_scridx()) return;
  switch (movtype)
    {
    case DOWN:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posy==amap->get_height()-1)||amap->is_unreachable(posx,posy+1)||
	  (amap->is_occuped(posx,posy+1))||
	  (amap->is_solid_down(posx,posy))) movtype=0;
      else amap->put_character(posx,posy+1,this);
      framefactor=0;
      break;
    case UP:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posy==0)||amap->is_unreachable(posx,posy-1)||
	  (amap->is_occuped(posx,posy-1))||
	  (amap->is_solid_up(posx,posy))) movtype=0;
      else amap->put_character(posx,posy-1,this);
      framefactor=2;
      break;
    case RIGHT:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posx==amap->get_lenght()-1)||amap->is_unreachable(posx+1,posy)||
	  (amap->is_occuped(posx+1,posy))||
	  (amap->is_solid_right(posx,posy))) movtype=0;
      else amap->put_character(posx+1,posy,this);
      framefactor=4;
      break;
    case LEFT:
      is_event=MAPCHAR_EVENT_LEFT;
      if ((posx==0)||amap->is_unreachable(posx-1,posy)||
	  (amap->is_occuped(posx-1,posy))||
	  (amap->is_solid_left(posx,posy))) movtype=0;
      else amap->put_character(posx-1,posy,this);
      framefactor=6;
      break;
    }
}

void mapcharacter::update_pos(landmap *amap)
{
  if (movtype!=0) speedcounter++;
  switch(movtype)
    {
    case RIGHT:
      if (!is_speed_ready()) break;
      scridx++;
      addx++;
      if (scridx==16) 
	{
	  scridx=0; 
	  addx=0 ;
	  posx++;
	  amap->leave_character(posx-1,posy);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case LEFT:
      if (!is_speed_ready()) break;
      if(scridx==0)
	{
	  scridx=addx=16;
	  posx--;
	}
      scridx--;
      addx--;
      if (scridx==0)
	{
	  amap->leave_character(posx+1,posy);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case UP:
      if (!is_speed_ready()) break;
      if(scridx==0)
	{
	  scridx=addy=16;
	  posy--;
	}
      scridx--;
      addy--;
      if (scridx==0)
	{
	  amap->leave_character(posx,posy+1);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    case DOWN:
      if (!is_speed_ready()) break;
      scridx++;
      addy++;
      if (scridx==16) 
	{
	  scridx=0; 
	  addy=0 ;
	  posy++;
	  amap->leave_character(posx,posy-1);
	  is_event=MAPCHAR_EVENT_COME;
	}
      break;
    }
}

void mapcharacter::update_eventleave(landmap *amap)
{
  if(!is_event) return;
  if(is_event==MAPCHAR_EVENT_LEFT)
    {
      is_event=0;
      amap->launch_event
	(this,amap->get_square_eventleavenbr(get_posx(),get_posy()));
      return;
    }
}

void mapcharacter::update_eventcome(landmap *amap)
{
  if(!is_event) return;
  if(is_event==MAPCHAR_EVENT_COME)
    {
      is_event=0;
      amap->launch_event
	(this,amap->get_square_eventcomenbr(get_posx(),get_posy()));
      return;
    }
}
