/*
   $Id$

   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef WIN_ANIM_H_
#define WIN_ANIM_H_


#include "animation.h"

class win_base;
class animation;
class win_container;
class win_select;


class win_anim : public win_base
{
  public:
  
  win_anim(s_int16,s_int16,animation * tpic,win_container * wc);
  void set_anim(animation *);
  void draw();
  void update();
  ~win_anim();
 
 
 private:
  animation * picture;
  
};
#endif
