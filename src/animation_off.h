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

#ifndef _ANIMATION_OFF_H
#define _ANIMATION_OFF_H

#include "animation.h"

class animation_off : public animation
{
  s_int16 xoffset, yoffset;
 public:
  animation_off();
  ~animation_off();
  animation_off & operator =(animation &a);
  animation_off & operator =(animation_off &a);

  s_int8 get(gzFile file);
  s_int8 load(const char * fname);
  
#ifdef _EDIT_
  s_int8 put(gzFile file);
  s_int8 save(const char * fname);
#endif
  void set_offset(s_int16 x, s_int16 y);
  void draw(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);
  void draw_border(u_int16 x, u_int16 y, drawing_area * da_opt=NULL);

  friend class mapobject;
  friend class mapcharacter;
};

#endif
