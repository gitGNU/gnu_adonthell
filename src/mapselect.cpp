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


#include "mapselect.h"

mapselect::mapselect(u_int16 x, u_int16 y, u_int16 l, u_int16 h,
		     u_int16 d_l, u_int16 d_h)
{
  s_posx=x;
  s_posy=y;
  length=l;
  height=h;
  posx=posy=d_posx=d_posy=0;
  dl=d_l;
  dh=d_h;
  cl=1;
  ch=1;
  cursor_blink=0;
  da=new drawing_area(s_posx,s_posy,dl*MAPSQUARE_SIZE,dh*MAPSQUARE_SIZE);
}

mapselect::~mapselect()
{
  delete da;
}

void mapselect::check_if_fits()
{
  if(posx>=length) posx=length-1;
  if(d_posx>=length) d_posx=length-1;
  if(posy>=height) posy=height-1;
  if(d_posy>=height) d_posy=height-1;
}

void mapselect::resize(u_int16 l, u_int16 h)
{
  length=l;
  height=h;
  check_if_fits();
  da->resize(dl*MAPSQUARE_SIZE,dh*MAPSQUARE_SIZE);
}

void mapselect::resize_view(u_int16 l, u_int16 h)
{
  dl=l;
  dh=h;
  check_if_fits();
  da->resize(dl*MAPSQUARE_SIZE,dh*MAPSQUARE_SIZE);
}

void mapselect::move(u_int16 x, u_int16 y)
{
  s_posx=x;
  s_posy=y;
  da->move(s_posx,s_posy);
}

void mapselect::set_cursor_pos(u_int16 x, u_int16 y)
{
  posx=x;
  posy=y;
  cursor_blink=0;
}

s_int8 mapselect::move_cursor_right()
{
  if(posx==length-1) return -1;
  posx++;
  if (posx>=d_posx+dl) d_posx++;
  cursor_blink=0;
  return 0;
}

s_int8 mapselect::move_cursor_left()
{
  if(posx==0) return -1;
  posx--;
  if(posx<d_posx) d_posx--;
  cursor_blink=0;
  return 0;
}

s_int8 mapselect::move_cursor_up()
{
  if(posy==0) return -1;
  posy--;
  if(posy<d_posy) d_posy--;
  cursor_blink=0;
  return 0;
}

s_int8 mapselect::move_cursor_down()
{
  if(posy==height-1) return -1;
  posy++;
  if (posy>=d_posy+dh) d_posy++;
  cursor_blink=0;
  return 0;
}

u_int16 mapselect::get_cursor_x()
{
  return posx;
}

u_int16 mapselect::get_cursor_y()
{
  return posy;
}

void mapselect::draw_cursor()
{
  if(cursor_blink<CURSOR_BLINK_RATE)
    screen::drawbox(s_posx+MAPSQUARE_SIZE*(posx-d_posx),
		    s_posy+MAPSQUARE_SIZE*(posy-d_posy),MAPSQUARE_SIZE,
		    MAPSQUARE_SIZE,0xFFFFFF);
}

void mapselect::update()
{
  if(++cursor_blink>=CURSOR_BLINK_RATE*2) cursor_blink=0;
}

void mapselect::draw_grid()
{
  static u_int16 i,j;
  for(i=d_posx;(i<d_posx+dl+1)&&(i<=length);i++)
    screen::drawbox(s_posx+((i-d_posx)*MAPSQUARE_SIZE),s_posy,1,dh*MAPSQUARE_SIZE+1,0xFFFFFF);
  for(j=d_posy;(j<d_posy+dh+1)&&(j<=height);j++)
    screen::drawbox(s_posx,s_posy+((j-d_posy)*MAPSQUARE_SIZE),dl*MAPSQUARE_SIZE+1,1,0xFFFFFF);
}

void mapselect::draw()
{
  draw_grid();
  draw_cursor();
}
