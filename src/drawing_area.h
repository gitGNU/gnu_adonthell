/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef _drawing_area_h
#define _drawing_area_h

#include "types.h"
#include "prefs.h"

class drawing_area : public SDL_Rect
{
  drawing_area * draw_to;
  SDL_Rect get_rects();
 public:

  drawing_area(u_int16 px, u_int16 py, u_int16 pw, u_int16 ph);
  drawing_area();
  void resize(u_int16 dl, u_int16 dh);
  void move(u_int16 dx, u_int16 dy);
  bool is_x_in(s_int16 px);
  bool is_y_in(s_int16 py);
  bool is_point_in(s_int16 px, s_int16 py);
  void assign_drawing_area(drawing_area * da);
  void detach_drawing_area();
  drawing_area &operator = (const SDL_Rect & r);
  drawing_area operator + (drawing_area & da);

  friend class image;
};

#endif
