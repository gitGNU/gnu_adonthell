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

#include <list>
#include "types.h"
#include "animation.h"
#include "win_base.h"

#include "win_container.h"
#include "win_select.h"
#include "win_anim.h"


win_anim::win_anim(s_int16 tx,s_int16 ty,animation * tpic,win_container * twc):win_base(tx,ty,0,0,twc,twc->get_drawing_area())
{
  picture=tpic;
  if(picture)
    {
      height=picture->t_frame[picture->frame[picture->currentframe].imagenbr].height;
      length=picture->t_frame[picture->frame[picture->currentframe].imagenbr].length;
    }
}

win_anim::~win_anim()
{
  dettach_select();
}

void win_anim::set_anim(animation * tpic)
{
  picture=tpic;
  if(picture)
    {
      height=picture->t_frame[picture->frame[picture->currentframe].imagenbr].height;
      length=picture->t_frame[picture->frame[picture->currentframe].imagenbr].length;
    }
}

void win_anim::draw()
{
  if(visible && wc && picture)
    {
       picture->draw(real_x,real_y,wc->get_drawing_area());
       draw_border();
    }
}

void win_anim::update()
{
  if(picture)
    {
      picture->update();
      height=picture->t_frame[picture->frame[picture->currentframe].imagenbr].height;
      length=picture->t_frame[picture->frame[picture->currentframe].imagenbr].length;
    } 
}





void win_anim::attach_select(win_select * tmp)
{
   wselect=tmp;
}


void win_anim::dettach_select()
{
  if(wselect)
    {
      wselect->remove(this);
      wselect=NULL;
    }
}
