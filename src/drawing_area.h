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

#ifndef _drawing_area_h
#define _drawing_area_h

#include "types.h"

class rect
{
 public:
  s_int16 length() { return length_; }
  s_int16 height() { return height_; }
  s_int16 x() { return x_; }
  s_int16 y() { return y_; }

  void move(s_int16 nx, s_int16 ny) { x_=nx; y_=ny; }
  void resize(s_int16 nl, s_int16 nh) { length_=nl; height_=nh; }

 protected:
  s_int16 x_,y_,length_,height_;
};

class drawing_area : public rect
{
 public:
  drawing_area();
#ifndef SWIG
  drawing_area(s_int16 px, s_int16 py, s_int16 pw, s_int16 ph);
#endif
  void assign_drawing_area(drawing_area * da) { draw_to=da; }
  void detach_drawing_area() { draw_to=NULL; }

#ifndef SWIG
  drawing_area &operator = (const rect & r);
  drawing_area operator + (drawing_area & da);
#endif

 protected:
  drawing_area * draw_to;

  rect get_rects();
  bool is_x_in(s_int16 px);
  bool is_y_in(s_int16 py);
  bool is_point_in(s_int16 px, s_int16 py);
 
#ifndef SWIG
  friend class image;
  friend class screen;
#endif
};

#endif
