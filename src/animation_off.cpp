/*
   $Id$

   Copyright (C) 1999/2000   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "animation_off.h"

animation_off::animation_off() : animation()
{
  xoffset=0;
  yoffset=0;
}

animation_off::~animation_off()
{
}

animation_off & animation_off::operator =(animation &a)
{
  *((animation*)this)=a;
  xoffset=yoffset=0;
  return *this;
}

animation_off & animation_off::operator =(animation_off &a)
{
  *((animation*)this)=a;
  xoffset=a.xoffset;
  yoffset=a.yoffset;
  return *this;
}

void animation_off::set_offset(s_int16 x, s_int16 y)
{
  xoffset=x;
  yoffset=y;
}

s_int8 animation_off::get(gzFile file)
{
  gzread(file,&xoffset,sizeof(xoffset));
  gzread(file,&yoffset,sizeof(yoffset));
  animation::get(file);
  return 0;
}

s_int8 animation_off::load(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"rb"); 
  if(!file) return -1;
  retvalue=get(file);
  gzclose(file);
  return retvalue;
}

#ifdef _EDIT_
s_int8 animation_off::put(gzFile file)
{
  gzwrite(file,&xoffset,sizeof(xoffset));
  gzwrite(file,&yoffset,sizeof(yoffset));
  animation::put(file);
  return 0;
}

s_int8 animation_off::save(const char * fname)
{
  gzFile file;
  u_int8 retvalue;
  file=gzopen(fname,"wb6"); 
  if(!file) return -1;
  retvalue=put(file);
  gzclose(file);
  return retvalue;
}
#endif

void animation_off::draw(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  animation::draw(x+xoffset,y+yoffset,da_opt);
}

void animation_off::draw_border(u_int16 x, u_int16 y, 
				drawing_area * da_opt=NULL)
{
  screen::drawbox(x+xoffset,y+yoffset,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset,y+yoffset+height,length,1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset+length,y+yoffset,1,height+1,0xFFFFFF,da_opt);
  screen::drawbox(x+xoffset,y+yoffset,1,height+1,0xFFFFFF,da_opt);
}
