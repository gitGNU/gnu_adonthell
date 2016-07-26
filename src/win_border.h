/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_BORDER_H_
#define WIN_BORDER_H_

#include <string.h>
#include "win_types.h"
#include "image.h"

class win_theme;
class win_base;

#define NB_BORDER_IMAGE 6

class win_border
{
 public:
  
  win_border();

  win_border(win_base * wb);
  
  win_border(win_border &);

  win_border(char *rep ,const char *size=WIN_BORDER_NORMAL_SIZE);
  
  ~win_border();
  
  void load(char *,const char *);
  
  void update();
  
  void destroy();

  void draw(drawing_area * da);
  
  void set_visible_border(bool b){visible_border_=b;}
  
  void set_brightness_border(bool b);
  
  void set_border(win_border & );

  void set_border(win_theme & wth, u_int8 size = win_border::NORMAL);

  void set_trans_border(bool b);

  u_int16 length_border();
  
  u_int16 height_border();

#ifdef SWIG
#define win_border_MINI 0
#define win_border_NORMAL 1
#else
    const static u_int8 MINI = 0;
    const static u_int8 NORMAL = 1;
#endif
 private:
  
  void init();

  void refresh();
  
  win_border & operator=(win_border &);

  image * h_border_template_;
  
  image * v_border_template_;
  
  image * border_[NB_BORDER_IMAGE];
  
  image * border_brightness_[NB_BORDER_IMAGE];

  image ** border_draw_;

  bool visible_border_;
  
  bool brightness_;

  u_int8 trans_;

  win_base * wb_;
};
#endif


