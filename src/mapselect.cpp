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

mapselect::mapselect(u_int16 x, u_int16 y, u_int16 l=0, u_int16 h=0)
{
  s_posx=x;
  s_posy=y;
  length=l;
  height=h;
  posx=posy=d_posx=s_posy=0;
}

mapselect::~mapselect()
{
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
}

void move(u_int16 x, u_int16 y)
{

}

void mapselect::update()
{

}

void mapselect::draw()
{

}
