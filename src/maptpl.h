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
  bool walkable;

  mapsquaretpl();
  s_int8 get(gzFile file);
  s_int8 put(gzFile file);
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
