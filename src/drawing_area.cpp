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

#include "drawing_area.h"

drawing_area::drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph)
{
  x=px;
  y=py;
  w=pw;
  h=ph;
  draw_to=NULL;
}

drawing_area::drawing_area()
{
  x=0;
  y=0;
  w=0;
  h=0;
  draw_to=NULL;
}

void drawing_area::resize(u_int16 dl, u_int16 dh)
{
  w=dl;
  h=dh;
}

void drawing_area::move(u_int16 dx, u_int16 dy)
{
  x=dx;
  y=dy;
}

bool drawing_area::is_x_in(s_int16 px)
{
  return((px-x>0)&&(px-(x+w)<0));
}

bool drawing_area::is_y_in(s_int16 py)
{
  return((py-y>0)&&(py-(y+h)<0));
}

bool drawing_area::is_point_in(s_int16 px, s_int16 py)
{
  return(is_x_in(px) && is_y_in(py));
}

void drawing_area::assign_drawing_area(drawing_area * da)
{
  draw_to=da;
}

void drawing_area::detach_drawing_area()
{
  draw_to=NULL;
}

SDL_Rect drawing_area::get_rects()
{
  if(draw_to)
    {
      SDL_Rect ret;
      SDL_Rect temp=draw_to->get_rects();
      ret.x=temp.x>x?temp.x:x;
      ret.y=temp.y>y?temp.y:y;
      ret.w=(temp.x+temp.w)<(x+w)?
	temp.x+temp.w-ret.x>0?temp.x+temp.w-ret.x:0:
	x+w-ret.x>0?x+w-ret.x:0;
      ret.h=(temp.y+temp.h)<(y+h)?
	temp.y+temp.h-ret.y>0?temp.y+temp.h-ret.y:0:
	y+h-ret.y>0?y+h-ret.y:0;
      return ret;
    }
  else return *this;
}

drawing_area &drawing_area::operator = (const SDL_Rect & r)
{
  x=r.x;
  y=r.y;
  w=r.w;
  h=r.h;
  return *this;
}

drawing_area drawing_area::operator + (drawing_area & da)
{
  drawing_area ret, temp=(*this);
  SDL_Rect t;
  temp.assign_drawing_area(&da);
  t=temp.get_rects();
  ret=t;
  ret.draw_to=NULL;
  return ret;
}
