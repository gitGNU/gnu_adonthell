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
#define WALKABLE_SOUTH 1
#define WALKABLE_NORTH 2
#define WALKABLE_EAST 4
#define WALKABLE_WEST 8
#define NONE_WALKABLE 0
  u_int8 walkable;

  mapsquaretpl();
  s_int8 get(gzFile file);
  s_int8 put(gzFile file);
  bool is_walkable_west() { return walkable & WALKABLE_WEST; }
  bool is_walkable_east() { return walkable & WALKABLE_EAST; }
  bool is_walkable_north() { return walkable & WALKABLE_NORTH; }
  bool is_walkable_south() { return walkable & WALKABLE_SOUTH; }

  void set_walkable_west(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_WEST);
      else walkable|=WALKABLE_WEST;
    }
  void set_walkable_east(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_EAST);
      else walkable|=WALKABLE_EAST;
    }  
  void set_walkable_north(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_NORTH);
      else walkable|=WALKABLE_NORTH;
    }  
  void set_walkable_south(bool w)
    {
      if(!w) walkable&=(ALL_WALKABLE-WALKABLE_SOUTH);
      else walkable|=WALKABLE_SOUTH;
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
