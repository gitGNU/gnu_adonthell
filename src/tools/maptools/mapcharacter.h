/*
   $Id$

   Copyright (C) 1999/2000/2001   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


#ifndef _MAPOBJ_H
#define _MAPOBJ_H

#include <vector>
#include "../../animation.h"
#ifdef _EDIT_
#include "../../mapselect.h"
#endif

class mapcharacter
{
  typedef enum { STAND_NORTH, STAND_SOUTH, STAND_WEST, STAND_EAST,
		 WALK_NORTH, WALK_SOUTH, WALK_WEST, WALK_EAST, NBR_MOVES } 
  c_moves;

  c_moves current_move;

  vector<animation*> anim;

 public:
  void init();
  void clear();
  mapcharacter();
  ~mapcharacter();

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);

  void update();
  void draw(s_int16 x, s_int16 y, drawing_area * da_opt=NULL);

#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);

  void insert_anim(animation * an, c_moves pos);
#endif
};

#endif

