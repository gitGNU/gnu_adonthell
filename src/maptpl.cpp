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

#include "maptpl.h"

mapsquaretpl::mapsquaretpl()
{
  walkable=true;
}

s_int8 mapsquaretpl::get(gzFile file)
{
  gzread(file,&walkable,sizeof(walkable));
  return 0;
}

s_int8 mapsquaretpl::put(gzFile file)
{
  gzwrite(file,&walkable,sizeof(walkable));
  return 0;
}

maptpl::maptpl(u_int16 x, u_int16 y, u_int16 l, u_int16 h,
	       u_int16 d_l, u_int16 d_h) : mapselect(x,y,l,h,d_l,d_h)
{
  u_int16 i,j;
  placetpl=new (mapsquaretpl*)[length];
  for(i=0;i<length;i++) placetpl[i]=new mapsquaretpl[h];
  selimg=new image(MAPSQUARE_SIZE, MAPSQUARE_SIZE);
  selbaseimg=new image(MAPSQUARE_SIZE, MAPSQUARE_SIZE);
  for(i=0;i<selimg->get_length();i++)
    for(j=0;j<selimg->get_height();j++)
      {
	selimg->put_pix(i,j,0x0000FF);
	selbaseimg->put_pix(i,j,0x00FF00);
      }
  basex=0;
  basey=0;
  selimg->set_alpha(110);
  selbaseimg->set_alpha(110);
}

maptpl::~maptpl()
{
  u_int16 i;
  for(i=0;i<length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  delete selimg;
  delete selbaseimg;
}

maptpl& maptpl::operator =(const maptpl& mt)
{
  return *this;
}

void maptpl::resize(u_int16 l, u_int16 h)
{
  u_int16 i,j;
  mapsquaretpl ** oldplacetpl=placetpl;
  placetpl=new (mapsquaretpl*)[l];
  for(i=0;i<l;i++)
    placetpl[i]=new mapsquaretpl[h];
  for(i=0;i<l && i<length;i++)
    for(j=0;j<h && j<height;j++)
      placetpl[i][j]=oldplacetpl[i][j];
  for(i=0;i<length;i++)
    delete[] oldplacetpl[i];
  delete[] oldplacetpl;
  mapselect::resize(l,h);
}

s_int8 maptpl::get(gzFile file)
{
  u_int16 i,j;
  for(i=0;i<length;i++)
    delete[] placetpl[i];
  delete[] placetpl;
  gzread(file,&length,sizeof(length));
  gzread(file,&height,sizeof(height));
  placetpl=new (mapsquaretpl*)[length];
  for(i=0;i<length;i++)
    {
      placetpl[i]=new mapsquaretpl[height];
      for(j=0;j<height;j++)
	placetpl[i][j].get(file);
    }
   gzread(file,&basex,sizeof(basex));
   gzread(file,&basey,sizeof(basey));
  return 0;
}

s_int8 maptpl::put(gzFile file)
{
  u_int16 i,j;
  gzwrite(file,&length,sizeof(length));
  gzwrite(file,&height,sizeof(height));
  for(i=0;i<length;i++)
    for(j=0;j<height;j++)
      placetpl[i][j].put(file);
  gzwrite(file,&basex,sizeof(basex));
  gzwrite(file,&basey,sizeof(basey));
  return 0;
}

void maptpl::set_base_tile(u_int16 x, u_int16 y)
{
  basex=x;
  basey=y;
}

void maptpl::toggle_walkable()
{
  placetpl[posx][posy].walkable=placetpl[posx][posy].walkable?false:true;
}

void maptpl::draw_walkables()
{
  u_int16 i,j;
  for(i=d_posx;(i<d_posx+dl)&&(i<length);i++)
    for(j=d_posy;(j<d_posy+dh)&&(j<height);j++)
      {
	const u_int32 col=0x0ff000;
	if(placetpl[i][j].is_walkable_left())
	  screen::drawbox(s_posx+((i-d_posx)*MAPSQUARE_SIZE+1),
			  s_posy+((j-d_posy)*MAPSQUARE_SIZE+1),
			  1,MAPSQUARE_SIZE-2,col);

	if(placetpl[i][j].is_walkable_right())
	  screen::drawbox(s_posx+((i-d_posx)*MAPSQUARE_SIZE)+MAPSQUARE_SIZE-1,
			  s_posy+((j-d_posy)*MAPSQUARE_SIZE+1),
			  1,MAPSQUARE_SIZE-2,col);
	
	if(placetpl[i][j].is_walkable_up())
	  screen::drawbox(s_posx+((i-d_posx)*MAPSQUARE_SIZE+1),
			  s_posy+((j-d_posy)*MAPSQUARE_SIZE+1),
			  MAPSQUARE_SIZE-2,1,col);
	
	if(placetpl[i][j].is_walkable_down())
	  screen::drawbox(s_posx+((i-d_posx)*MAPSQUARE_SIZE)+1,
			  s_posy+((j-d_posy)*MAPSQUARE_SIZE)+MAPSQUARE_SIZE-1,
			  MAPSQUARE_SIZE-2,1,col);

      }
}

void maptpl::draw_base_tile()
{
  if((basex>=d_posx && basex<d_posx+dl)&&
     (basey>=d_posy && basey<d_posy+dh))
    selbaseimg->draw(s_posx+(basex-d_posx)*MAPSQUARE_SIZE,
		     s_posy+(basey-d_posy)*MAPSQUARE_SIZE);
}

void maptpl::draw_base_tile(u_int16 x, u_int16 y, drawing_area * da_opt=NULL)
{
  selbaseimg->draw(x,y,da_opt);
}

void maptpl::draw()
{
  mapselect::draw();
  draw_base_tile();
  draw_walkables();
}
