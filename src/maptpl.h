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


#ifndef _MAPTPL_H
#define _MAPTPL_H

#include "mapselect.h"

class mapsquaretpl
{
 public:
#define ALL_WALKABLE 15
#define WALKABLE_DOWN 1
#define WALKABLE_UP 2
#define WALKABLE_RIGHT 4
#define WALKABLE_LEFT 8
#define NONE_WALKABLE 0
  u_int8 walkable;

  mapsquaretpl();
  s_int8 get(gzFile file);
  s_int8 put(gzFile file);
  bool is_walkable_left() { return walkable & WALKABLE_LEFT; }
  bool is_walkable_right() { return walkable & WALKABLE_RIGHT; }
  bool is_walkable_up() { return walkable & WALKABLE_UP; }
  bool is_walkable_down() { return walkable & WALKABLE_DOWN; }
  void set_walkable_left(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_LEFT);
      else walkable|=WALKABLE_LEFT;
    }
  void set_walkable_right(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_RIGHT);
      else walkable|=WALKABLE_RIGHT;
    }  
  void set_walkable_up(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_UP);
      else walkable|=WALKABLE_UP;
    }  
  void set_walkable_down(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_DOWN);
      else walkable|=WALKABLE_DOWN;
    }
  
  friend class maptpl;
  
};

class maptpl : public mapselect
{
 public:
  mapsquaretpl ** placetpl;
  u_int16 basex, basey;

  // FIXME: should to go mapselect!
  image * selimg;
  image * selbaseimg;

  maptpl(u_int16 x, u_int16 y, u_int16 l, u_int16 h,
	 u_int16 d_l, u_int16 d_h);
  ~maptpl();
  
  maptpl& operator =(const maptpl& mt);

  s_int8 get(gzFile file);
  s_int8 put(gzFile file);
  void resize(u_int16 l, u_int16 h);
  void set_base_tile(u_int16 x, u_int16 y);

  void toggle_walkable();

  void draw_walkables();
  void draw_base_tile();
  void draw_base_tile(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);
  void draw();
};

#endif
